#include "firstQuery.hpp"
#include <kDataFrame.hpp>
#include <algorithms.hpp>
#include <string>
#include <assert.h>
#include <tuple>
#include <omp.h>
#include <chrono>

using namespace std;


inline string kmers_to_seq(vector<kmer_row> &kmers) {
    string seq;
    int kSize = kmers[0].str.size();
    int i;
    for (i = 0; i < kmers.size(); i += kSize)
        seq += kmers[i].str;

    return seq.substr(0, (kmers.size() - 1)) + kmers.back().str;
}

tuple<string, bool, int, uint64_t> firstQuery::classifyQuery(std::vector<kmer_row> &kmers, int PE) {

    /*
     * scenarios:
        1 "Mapped: from matching the first and last kmers only."
        2 "Unmapped: Both terminal kmers matched but on different components."
        3 "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched."
        4 "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1."
        5 "Mapped: Partial match and read is trimmed."
        6 "Unmapped: There's no single matched kmer."
     * */

    string constructed_read;
    int scenario = 0;
    int collectiveComponent;
    uint64_t color1 = this->kf->getCount(kmers.front().hash);
    uint64_t color2 = this->kf->getCount(kmers.back().hash);

//    constructed_read = kmers_to_seq(kmers);
//    cout << constructed_read << endl;
//    return make_tuple(constructed_read, true, 1, 1);

    if (color1 != 0 && color2 != 0) {

        if (color1 == color2) {
            constructed_read = kmers_to_seq(kmers);
            scenario = 1;
            this->scenarios_count[PE][scenario]++;
            collectiveComponent = color1;
            return make_tuple(constructed_read, true, scenario, collectiveComponent);

        } else {
            scenario = 2;
            this->scenarios_count[PE][scenario]++;
            constructed_read = kmers_to_seq(kmers);
            return make_tuple(constructed_read, false, scenario, 0);
        }
    } else {
        // That mean both are zeros so both could not be found
        // Case 3
        double noKmers = (double) kmers.size();

        vector<uint64_t> all_colors;

        phmap::flat_hash_set<uint64_t> unique_colors;
        double found_count = 0;


        // Get all the colors
        for (const auto &kmer: kmers) {
            uint64_t color = kf->getCount(kmer.hash); // This line is a landmine, take care.
            if (color != 0) {
                found_count++;
            }
            all_colors.push_back(color);
            unique_colors.insert(color);
        }


        // Check found Vs. unfound
        if ((found_count / noKmers) < 0.5) {
            // not aligned read

            scenario = 3;
            this->scenarios_count[PE][scenario]++;
            constructed_read = kmers_to_seq(kmers);
            return make_tuple(constructed_read, false, scenario, 0);

        } else {

/*
            for(auto & color : all_colors){
                cout << color << ",";
            }

            cout << endl;
            cout <<  endl;
*/

            if (unique_colors.size() > 2) {
                // unfound = 0, found = !0, so if all the kmers are coming from single component then the unique number of colors should be 2

                scenario = 4;
                this->scenarios_count[PE][scenario]++;
                constructed_read = kmers_to_seq(kmers);
                return make_tuple(constructed_read, false, scenario, 0);
            } else if (unique_colors.size() == 2) { // This is important, to assure there's an exact one color found.

                int start_kmer, end_kmer;
                start_kmer = 0;
                end_kmer = 0;
                auto it = all_colors.begin();
                uint64_t collectiveComponent;

                for (auto _color : unique_colors) {
                    if (_color != 0) {
                        collectiveComponent = _color;
                        break;
                    }
                }

                while (it != all_colors.end()) {
                    if (*it != 0) {
                        // cout << "*it = " << *it << endl;
                        start_kmer = distance(all_colors.begin(), it);
                        // cout << "start_kmer = " << start_kmer << endl;
                        auto rev_it = all_colors.end();
                        while (rev_it != it) {
                            rev_it--;
                            if (*rev_it != 0) {
                                // cout << "*rev_it = " << *rev_it << endl;
                                end_kmer = distance(all_colors.begin(), rev_it);
                                // cout << "end_kmer = " << end_kmer << endl;
                                it = all_colors.end();
                                break;
                            }
                        }
                    } else {
                        it++;
                    }
                }

                scenario = 5;
                this->scenarios_count[PE][scenario]++;

                vector<kmer_row> sliced_kmers = std::vector<kmer_row>(kmers.begin() + start_kmer,
                                                                      kmers.begin() + end_kmer + 1);
                constructed_read = kmers_to_seq(sliced_kmers);

                return make_tuple(constructed_read, true, scenario, collectiveComponent);

            } else {
                scenario = 6;
                this->scenarios_count[PE][scenario]++;
                constructed_read = kmers_to_seq(kmers);
                return make_tuple(constructed_read, false, scenario, 0);
            }
        }
    }

}

void firstQuery::start_query() {

    int no_of_sequences = 67954363;


    int no_chunks = no_of_sequences / this->chunk_size;

    kmerDecoder *READ_1_KMERS = new Kmers(this->PE_1_reads_file, this->chunk_size, this->kSize);
    kmerDecoder *READ_2_KMERS = new Kmers(this->PE_2_reads_file, this->chunk_size, this->kSize);

    int Reads_chunks_counter = 0;

    while (!READ_1_KMERS->end() && !READ_2_KMERS->end()) {

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        READ_1_KMERS->next_chunk();
        READ_2_KMERS->next_chunk();


        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1 = READ_1_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2 = READ_2_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1_end = READ_1_KMERS->getKmers()->end();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2_end = READ_2_KMERS->getKmers()->end();

        while (seq1 != seq1_end && seq2 != seq2_end) {
            // assert(seq1->second.size() == 120);
            tuple<string, bool, int, uint64_t> read_1_result = this->classifyQuery(seq1->second, 1);
            tuple<string, bool, int, uint64_t> read_2_result = this->classifyQuery(seq2->second, 2);


            string read_1_constructedRead = get<0>(read_1_result);
            bool read_1_mapped_flag = get<1>(read_1_result);
            int read_1_collectiveComponent = get<2>(read_1_result);

            string read_2_constructedRead = get<0>(read_2_result);
            bool read_2_mapped_flag = get<1>(read_2_result);
            int read_2_collectiveComponent = get<2>(read_2_result);

            this->SQL->insert_PE(read_1_constructedRead, read_2_constructedRead, read_1_collectiveComponent, read_2_collectiveComponent);

            seq1++;
            seq2++;
        }


        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        long hr = milli / 3600000;
        milli = milli - 3600000 * hr;
        long min = milli / 60000;
        milli = milli - 60000 * min;
        long sec = milli / 1000;
        milli = milli - 1000 * sec;
        cerr << "processed chunk: (" << ++Reads_chunks_counter << ") / (" << no_chunks << ") in ";
        cout << min << ":" << sec << ":" << milli << endl;

    }


}
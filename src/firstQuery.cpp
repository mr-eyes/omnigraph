#include "firstQuery.hpp"
#include <kDataFrame.hpp>
#include <algorithms.hpp>
#include <string>



void firstQuery::start_query() {

    int no_chunks = 67954363 / this->chunk_size;

    kmerDecoder *READ_1_KMERS = new Kmers(this->PE_1_reads_file, this->chunk_size, this->kSize);
    kmerDecoder *READ_2_KMERS = new Kmers(this->PE_2_reads_file, this->chunk_size, this->kSize);

    int Reads_chunks_counter = no_chunks;

    while (!READ_1_KMERS->end() && !READ_2_KMERS->end()) {
        READ_1_KMERS->next_chunk();
        READ_2_KMERS->next_chunk();
        // --Reads_chunks_counter;
        // cerr << Reads_chunks_counter << " remain chunks ..." << endl;

        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1 = READ_1_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2 = READ_2_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1_end = READ_1_KMERS->getKmers()->end();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2_end = READ_2_KMERS->getKmers()->end();

        while (seq1 != seq1_end && seq2 != seq2_end) {

            uint64_t color1 = kf->getCount(seq1->second.front().hash);
            uint64_t color2 = kf->getCount(seq1->second.back().hash);

            if (color1 != 0 && color2 != 0) {

                if (color1 == color2) {
                    cout << seq1->first << '\t' << 1 << '\t' << "0:-1" << endl;
                } else {
                    cout << seq1->first << '\t' << 0 << '\t' << "0:0" << endl;
                }
            } else {
                // That mean both are zeros so both could not be found
                int noKmers = seq1->second.size();
                vector<uint64_t> all_colors(noKmers);

                phmap::flat_hash_set<uint64_t> unique_colors;
                phmap::flat_hash_map<bool, int> found_count;

                // Get all the colors
                for (const auto &kmer: seq1->second) {
                    uint64_t color = kf->getCount(kmer.hash);
                    found_count[bool(color)]++;
                    all_colors.push_back(color);
                    unique_colors.insert(color);
                }

                // Check found Vs. unfound
                if ((found_count[0] / noKmers) < 0.5) {
                    // not aligned read
                    cout << seq1->first << '\t' << 0 << '\t' << "0:0" << endl;
                } else {
                    if (unique_colors.size() > 2) {
                        // unfound = 0, found = !0, so if all the kmers are coming from single component then the unique number of colors should be 2
                        cout << seq1->first << '\t' << 0 << '\t' << "0:0" << endl;
                    } else {
                        int start_kmer, end_kmer, i, j;
                        i = 0;
                        j = 0;
                        auto it = all_colors.begin();

                        while (it != all_colors.end()) {
                            i++;
                            if (*it != 0) {
                                start_kmer = i;
                                auto rev_it = all_colors.end();
                                while (rev_it != it) {
                                    j--;
                                    if (*rev_it != 0) {
                                        end_kmer = j;
                                        it = all_colors.end();
                                        break;
                                    }
                                    --rev_it;
                                }
                            }
                            ++it;
                        }
                        cout << seq1->first << '\t' << 1 << '\t' << start_kmer << ':' << end_kmer << endl;
                    }
                }
            }
            seq1++;
            seq2++;
        }
    }



}
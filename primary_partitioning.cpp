// read_id      R1orR2      found_ratio     scenario_id(3,4,5,6)

#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include "omnigraph.hpp"
#include <cassert>
//#include "progressbar.hpp"
//#include "tqdm.h"


using namespace std;

class fileHandler {

public:
    ofstream fileStream;

    explicit fileHandler(string filename) {
        this->fileStream.open(filename);
    }

    void write(string &line) {
        this->fileStream << line;
    }

    void close() {
        fileStream.close();
    }

};


int main(int argc, char **argv) {

    // Index prefix
    // Read 1
    // Read 2
    string index_prefix, PE_1_reads_file, PE_2_reads_file, out_prefix;
    int batchSize = 10000;
    int kSize = 75;
    int no_of_sequences = 67954363;
    int hashing_mode = 3;

    // Temporary solution for the Farm IO
    if (argc < 4) {
        cerr << "run: ./primaryPartitioning <index_prefix> <PE_R1> <PE_R2> <out_prefix>" << endl;
        exit(1);
    } else {
        index_prefix = argv[1];
        PE_1_reads_file = argv[2];
        PE_2_reads_file = argv[3];
        out_prefix = argv[4];
    }

    cerr << "Processing: \nR1: " << PE_1_reads_file << "\nR2: " << PE_2_reads_file << endl;

    // Instantiations
    auto *partitioner = new Omnigraph();
    auto *detailed_stats_file = new fileHandler(out_prefix + "_detailed_stats.tsv");
    string header = "ID\tR\tfound_kmers%\tscenario\n";
    detailed_stats_file->write(header);

    kmerDecoder *READ_1_KMERS = new Kmers(PE_1_reads_file, batchSize, kSize);
    kmerDecoder *READ_2_KMERS = new Kmers(PE_2_reads_file, batchSize, kSize);

    READ_1_KMERS->setHashingMode(hashing_mode);
    READ_2_KMERS->setHashingMode(hashing_mode);

    // Initializations
    int no_chunks = ceil((double) no_of_sequences / (double) batchSize);
    int current_chunk = 0;


    // kProcessor Index Loading
    std::cerr << "Loading labeled cDBG ..." << std::endl;
    kDataFrame *kf = kDataFrame::load(index_prefix);
    assert(kSize == (int) kf->getkSize());
    std::cerr << "Labeled cDBG loaded successfully ..." << std::endl;


    while (!READ_1_KMERS->end() && !READ_2_KMERS->end()) {

        cerr << "processing chunk: (" << ++current_chunk << ") / (" << no_chunks << ") ... ";
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        READ_1_KMERS->next_chunk();
        READ_2_KMERS->next_chunk();


        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1 = READ_1_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2 = READ_2_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1_end = READ_1_KMERS->getKmers()->end();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2_end = READ_2_KMERS->getKmers()->end();

        vector<tuple<string, int, double, int>> detailed_chunk_stats;
        // tuple<seq, matched, scenario, component, found_ratio>
        // read_id      R1orR2      found_ratio     scenario_id(3,4,5,6)
        while (seq1 != seq1_end && seq2 != seq2_end) {
            tuple<string, bool, int, uint32_t, double> read_1_result = partitioner->classifyRead_withStats(kf,seq1->second,1);
            tuple<string, bool, int, uint32_t, double> read_2_result = partitioner->classifyRead_withStats(kf,seq2->second,2);

            string read_1_constructedRead = get<0>(read_1_result);
//            bool read_1_mapped_flag = get<1>(read_1_result);
            int read_1_scenario = get<2>(read_1_result);
//            uint32_t read_1_collectiveComponent = get<3>(read_1_result);
            double read_1_found_ratio = get<4>(read_1_result);
            detailed_chunk_stats.push_back(make_tuple(seq1->first, 1, read_1_found_ratio, read_1_scenario));

            string read_2_constructedRead = get<0>(read_2_result);
//            bool read_2_mapped_flag = get<1>(read_2_result);
            int read_2_scenario = get<2>(read_2_result);
//            uint32_t read_2_collectiveComponent = get<3>(read_2_result);
            double read_2_found_ratio = get<4>(read_2_result);
            detailed_chunk_stats.push_back(make_tuple(seq2->first, 2, read_2_found_ratio, read_2_scenario));

            seq1++;
            seq2++;
        }

        // Writing detailed stats
        // read_id      R1orR2      found_ratio     scenario_id(3,4,5,6)
        for (const auto &row : detailed_chunk_stats) {
            string line = get<0>(row) + "\t" + to_string(get<1>(row)) + "\t" + to_string(get<2>(row)) + "\t" +
                          to_string(get<3>(row)) + "\n";
            detailed_stats_file->write(line);
        }
        detailed_chunk_stats.clear();


        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        long hr = milli / 3600000;
        milli = milli - 3600000 * hr;
        long min = milli / 60000;
        milli = milli - 60000 * min;
        long sec = milli / 1000;
        milli = milli - 1000 * sec;
        cerr << "Done in: ";
        cout << min << ":" << sec << ":" << milli << endl;
    }

    cout << endl << endl;
    cout << "Summary report: " << endl << endl;

    // Printing a summary report

    for (int p = 1; p <= 2; p++) {
        double total = 0;
        for (int scenario = 1; scenario <= 6; scenario++)
            total += partitioner->scenarios_count[p][scenario];

        double mapped_percentage = (partitioner->scenarios_count[p][1] + partitioner->scenarios_count[p][5]);
        mapped_percentage = (mapped_percentage / total) * 100;

        cout << "Paired End File: " << p << " | mapped_reads  %" << mapped_percentage << endl;
        for (int scenario = 1; scenario <= 6; scenario++) {
            int count = partitioner->scenarios_count[p][scenario];
            string description = partitioner->scenario_descriptions[scenario];
            cout << "Scenario (" << scenario << ") : Count: " << count << " | " << description << endl;
        }
        cout << "---------------------------------" << endl;
    }


    delete kf;
    delete READ_1_KMERS;
    delete READ_2_KMERS;
    detailed_stats_file->close();

    return 0;
}

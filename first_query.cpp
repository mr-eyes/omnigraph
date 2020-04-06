#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithms.hpp>
#include <firstQuery.hpp>
#include "INIReader.h"
#include "omnigraph.hpp"
#include "assert.h"

using namespace std;

int main(int argc, char **argv) {

    string config_file_path = "../config.ini";
    string index_prefix, PE_1_reads_file, PE_2_reads_file, sqlite_db;
    int batchSize, kSize, no_of_sequences;

    INIReader reader(config_file_path);

    if (reader.ParseError() != 0) {
        std::cout << "Can't load '" << config_file_path << "'\n";
        return 1;
    }


    index_prefix = reader.Get("kProcessor", "idx_prefix", "");
    PE_1_reads_file = reader.Get("Reads", "read1", "");
    PE_2_reads_file = reader.Get("Reads", "read2", "");
    no_of_sequences = reader.GetInteger("Reads", "seqs_no", 0);
    sqlite_db = reader.Get("SQLite", "db_file", "query1_result.db");
    batchSize = reader.GetInteger("kProcessor", "chunk_size", 1);
    kSize = reader.GetInteger("kProcessor", "ksize", 31);

    cerr << "Processing: \nR1: " << PE_1_reads_file << "\nR2: " << PE_2_reads_file << endl;

    // Instantiations
    Omnigraph *first_query = new Omnigraph();
    SQLiteManager *SQL = new SQLiteManager(sqlite_db);
    kmerDecoder *READ_1_KMERS = new Kmers(PE_1_reads_file, batchSize, kSize);
    kmerDecoder *READ_2_KMERS = new Kmers(PE_2_reads_file, batchSize, kSize);

    // Initializations
    int no_chunks = no_of_sequences / batchSize;
    int Reads_chunks_counter = 0;


    // kProcessor Index Loading
    std::cerr << "Loading index ..." << std::endl;
    colored_kDataFrame *ckf = colored_kDataFrame::load(index_prefix);
    kDataFrame *kf = ckf->getkDataFrame();
    assert(kSize == (int) kf->getkSize());
    std::cerr << "kProcessor index loaded successfully ..." << std::endl;


    while (!READ_1_KMERS->end() && !READ_2_KMERS->end()) {

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        READ_1_KMERS->next_chunk();
        READ_2_KMERS->next_chunk();


        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1 = READ_1_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2 = READ_2_KMERS->getKmers()->begin();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq1_end = READ_1_KMERS->getKmers()->end();
        flat_hash_map<std::string, std::vector<kmer_row>>::iterator seq2_end = READ_2_KMERS->getKmers()->end();

        while (seq1 != seq1_end && seq2 != seq2_end) {
            tuple<string, bool, int, uint64_t> read_1_result = first_query->classifyRead(kf, seq1->second, 1);
            tuple<string, bool, int, uint64_t> read_2_result = first_query->classifyRead(kf, seq2->second, 2);

            string read_1_constructedRead = get<0>(read_1_result);
            bool read_1_mapped_flag = get<1>(read_1_result);
            int read_1_collectiveComponent = get<2>(read_1_result);

            string read_2_constructedRead = get<0>(read_2_result);
            bool read_2_mapped_flag = get<1>(read_2_result);
            int read_2_collectiveComponent = get<2>(read_2_result);

            SQL->insert_PE(read_1_constructedRead, read_2_constructedRead, read_1_collectiveComponent,
                           read_2_collectiveComponent);

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



    // Printing a summary report

    for (int p = 1; p <= 2; p++) {
        double total = 0;
        for (int scenario = 1; scenario <= 6; scenario++)
            total += first_query->scenarios_count[p][scenario];

        double mapped_percentage = (first_query->scenarios_count[p][1] + first_query->scenarios_count[p][5]);
        mapped_percentage = (mapped_percentage / total) * 100;

        cout << "Paired End File: " << p << " | mapped_reads  %" << mapped_percentage << endl;
        for (int scenario = 1; scenario <= 6; scenario++) {
            int count = first_query->scenarios_count[p][scenario];
            string description = first_query->scenario_descriptions[scenario];
            cout << "Scenario (" << scenario << ") : Count: " << count << " | " << description << endl;
        }
        cout << "---------------------------------" << endl;
    }

    SQL->close();

    return 0;
}

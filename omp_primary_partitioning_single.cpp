#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <firstQuery.hpp>
#include "INIReader.h"
#include "omnigraph.hpp"
#include "assert.h"
#include <omp.h>

using namespace std;

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

    string sqlite_db = out_prefix + "_omni.db";

    cerr << "Processing: \nR1: " << PE_1_reads_file << "\nR2: " << PE_2_reads_file << endl;

    // Instantiations
    Omnigraph *originalCompsQuery = new Omnigraph();
    SQLiteManager *SQL = new SQLiteManager(sqlite_db);
    SQL->create_reads_table(originalCompsQuery->partitioning_mode);

    // Instantiate kmerDecoder with hashing mode 3
    kmerDecoder *READ_1_KMERS = new Kmers(PE_1_reads_file, batchSize, kSize);
    READ_1_KMERS->setHashingMode(hashing_mode);
    kmerDecoder *READ_2_KMERS = new Kmers(PE_2_reads_file, batchSize, kSize);
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

        vector <tuple<string, string, uint32_t, uint32_t>> sqlite_chunk; // Buffer for holding Sqlite rows

        tuple<string, bool, int, uint64_t> read_1_result;
        tuple<string, bool, int, uint64_t> read_2_result;

        while (seq1 != seq1_end && seq2 != seq2_end) {
#pragma omp parallel
            {
#pragma omp task
                read_1_result = originalCompsQuery->classifyRead(kf, seq1->second,
                                                                                                    1);

#pragma omp task
                read_2_result = originalCompsQuery->classifyRead(kf, seq2->second,
                                                                                                    2);
            }

            string R1_constructedRead = get<0>(read_1_result);
//            bool read_1_mapped_flag = get<1>(read_1_result);
            uint32_t R1_connectedComponent = get<3>(read_1_result);

            string R2_constructedRead = get<0>(read_2_result);
//            bool read_2_mapped_flag = get<1>(read_2_result);
            uint32_t R2_connectedComponent = get<3>(read_2_result);

//            SQL->insert_PE(R1_constructedRead, R2_constructedRead, R1_connectedComponent,
//                           R2_connectedComponent);

            sqlite_chunk.emplace_back(
                    make_tuple(R1_constructedRead, R2_constructedRead, R1_connectedComponent, R2_connectedComponent));

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
        cerr << "Done in: ";
        cerr << min << ":" << sec << ":" << milli;


        // --------------------------------------------------------------------------------
        //                                      SQLITE Insertion                          |
        // --------------------------------------------------------------------------------

        sqlite3_mutex_enter(sqlite3_db_mutex(SQL->db.db_));
        char *errorMessage;
        sqlite3_exec(SQL->db.db_, "PRAGMA synchronous=OFF", nullptr, nullptr, &errorMessage);
        sqlite3_exec(SQL->db.db_, "PRAGMA count_changes=OFF", nullptr, nullptr, &errorMessage);
        sqlite3_exec(SQL->db.db_, "PRAGMA journal_mode=MEMORY", nullptr, nullptr, &errorMessage);
        sqlite3_exec(SQL->db.db_, "PRAGMA temp_store=MEMORY", nullptr, nullptr, &errorMessage);
        sqlite3_exec(SQL->db.db_, "BEGIN TRANSACTION", nullptr, nullptr, &errorMessage);
        char const *szSQL = "INSERT INTO reads (PE_seq1, PE_seq2, seq1_original_component, seq2_original_component) VALUES (?,?,?,?);";
        sqlite3_stmt *stmt;
        SQL->rc = sqlite3_prepare(SQL->db.db_, szSQL, -1, &stmt, nullptr);


        if (SQL->rc == SQLITE_OK) {

            for (auto &row : sqlite_chunk) {
                char const *R1_seq = get<0>(row).c_str();
                char const *R2_seq = get<1>(row).c_str();

                sqlite3_bind_text(stmt, 1, R1_seq, strlen(R1_seq), nullptr);
                sqlite3_bind_text(stmt, 2, R2_seq, strlen(R2_seq), nullptr);
                sqlite3_bind_int64(stmt, 3, get<2>(row));
                sqlite3_bind_int64(stmt, 4, get<3>(row));

                int retVal = sqlite3_step(stmt);
                if (retVal != SQLITE_DONE) {
                    printf("Commit Failed! %d\n", retVal);
                }

                sqlite3_reset(stmt);
            }
            sqlite3_exec(SQL->db.db_, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
            sqlite3_finalize(stmt);

        } else {
            fprintf(stderr, "SQL error: %s\n", SQL->zErrMsg);
            sqlite3_free(SQL->zErrMsg);
        }
        sqlite3_mutex_leave(sqlite3_db_mutex(SQL->db.db_));

        // --------------------------------------------------------------------------------
        //                                      Done Insertion                          |
        // --------------------------------------------------------------------------------


        // Calculating Inserting Time Only
        std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
        milli = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
        hr = milli / 3600000;
        milli = milli - 3600000 * hr;
        min = milli / 60000;
        milli = milli - 60000 * min;
        sec = milli / 1000;
        milli = milli - 1000 * sec;
        cerr << " | inserted in: ";
        cerr << min << ":" << sec << ":" << milli;

        // Calculating Total Time
        std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
        milli = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t1).count();
        hr = milli / 3600000;
        milli = milli - 3600000 * hr;
        min = milli / 60000;
        milli = milli - 60000 * min;
        sec = milli / 1000;
        milli = milli - 1000 * sec;
        cerr << " | total : ";
        cerr << min << ":" << sec << ":" << milli << endl;

    }



    // Printing a summary report

    for (int p = 1; p <= 2; p++) {
        double total = 0;
        for (int scenario = 1; scenario <= 6; scenario++)
            total += originalCompsQuery->scenarios_count[p][scenario];

        double mapped_percentage = (originalCompsQuery->scenarios_count[p][1] +
                                    originalCompsQuery->scenarios_count[p][5]);
        mapped_percentage = (mapped_percentage / total) * 100;

        cout << "Paired End File: " << p << " | mapped_reads  %" << mapped_percentage << endl;
        for (int scenario = 1; scenario <= 6; scenario++) {
            int count = originalCompsQuery->scenarios_count[p][scenario];
            string description = originalCompsQuery->scenario_descriptions[scenario];
            cout << "Scenario (" << scenario << ") : Count: " << count << " | " << description << endl;
        }
        cout << "---------------------------------" << endl;
    }

    SQL->close();
    delete kf;
    delete READ_1_KMERS;
    delete READ_2_KMERS;

    return 0;
}

#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithms.hpp>
#include <firstQuery.hpp>
#include "INIReader.h"

using namespace std;

int main(int argc, char **argv) {

    string config_file_path = "../config.ini";
    string index_prefix, PE_1_reads_file, PE_2_reads_file, sqlite_db;
    int batchSize;

    INIReader reader(config_file_path);

    if (reader.ParseError() != 0) {
        std::cout << "Can't load '" << config_file_path << "'\n";
        return 1;
    }


    index_prefix = reader.Get("kProcessor", "idx_prefix", "");
    PE_1_reads_file = reader.Get("Reads", "read1", "");
    PE_2_reads_file = reader.Get("Reads", "read2", "");
    sqlite_db = reader.Get("SQLite", "db_file", "query1_result.db");
    batchSize = reader.GetInteger("kProcessor", "chunk_size", 1);

    cerr << "Processing: \nR1: " << PE_1_reads_file << "\nR2: " << PE_2_reads_file << endl;


    firstQuery *Query = new firstQuery(PE_1_reads_file, PE_2_reads_file, index_prefix, sqlite_db, batchSize);
    Query->start_query();

    for (int p = 1; p <= 2; p++) {
        double total = 0;
        for (int scenario = 1; scenario <= 6; scenario++)
            total += Query->scenarios_count[p][scenario];

        double mapped_percentage = (Query->scenarios_count[p][1] + Query->scenarios_count[p][5]);
        mapped_percentage = (mapped_percentage / total) * 100;

        cout << "Paired End File: " << p << " | mapped_reads  %" << mapped_percentage << endl;
        for (int scenario = 1; scenario <= 6; scenario++) {
            int count = Query->scenarios_count[p][scenario];
            string description = Query->scenario_descriptions[scenario];
            cout << "Scenario (" << scenario << ") : Count: " << count << " | " << description << endl;
        }
        cout << "---------------------------------" << endl;
    }


    return 0;
}

#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithms.hpp>
#include <firstQuery.hpp>

using namespace std;

int main(int argc, char **argv) {

    string index_prefix = "../data/idx_cDBG_SRR11015356_k31unitigs/idx_idx_cDBG_SRR11015356_k31unitigs";
    string PE_1_reads_file = "../data/SRR11015356_1.fasta";
    string PE_2_reads_file = "../data/SRR11015356_2.fasta";

    firstQuery *Query = new firstQuery(PE_1_reads_file, PE_2_reads_file, index_prefix, 50000);
    Query->start_query();

    for(int p = 1; p <= 2; p++){
        double total = 0;
        for(int scenario = 1; scenario <= 6; scenario++)
            total += Query->scenarios_count[p][scenario];

        double mapped_percentage = (Query->scenarios_count[p][1] + Query->scenarios_count[p][5]);
        mapped_percentage = (mapped_percentage / total) * 100;

        cout << "Paired End File: " << p << " | mapped_reads  %" << mapped_percentage << endl;
        for(int scenario = 1; scenario <= 6; scenario++){
            int count = Query->scenarios_count[p][scenario];
            string description = Query->scenario_descriptions[scenario];
            cout << "Scenario (" << scenario << ") : Count: " << count << " | " << description << endl;
        }
        cout << "---------------------------------" << endl;        
    }


    return 0;
}

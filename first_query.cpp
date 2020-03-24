#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithms.hpp>
#include <firstQuery.hpp>

using namespace std;

string kmers_to_seq1(vector<string> &kmers) {
    string seq1;
    int kSize = kmers[0].size();
    int i;
    for (i = 0; i < kmers.size(); i += kSize)
        seq1 += kmers[i];

    return seq1.substr(0, (kmers.size() - 1)) + kmers.back();
}

int main(int argc, char **argv) {

    string index_prefix = "../data/idx_cDBG_SRR11015356_k31unitigs/idx_idx_cDBG_SRR11015356_k31unitigs";
    string PE_1_reads_file = "../data/SRR11015356_1.fasta";
    string PE_2_reads_file = "../data/SRR11015356_2.fasta";

    firstQuery *Query = new firstQuery(PE_1_reads_file, PE_2_reads_file, index_prefix, 100000);
    Query->start_query();

    return 0;
}

#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithms.hpp>
#include <iostream>
#include <tuple>

using namespace std;

class firstQuery {

public:
    string PE_1_reads_file;
    string PE_2_reads_file;
    string index_prefix;
    int kSize, chunk_size;

    kDataFrame *kf;
    colored_kDataFrame *ckf;

    flat_hash_map<int , flat_hash_map <int, int>> scenarios_count =
        {
            {1 , 
                {
                        {1, 0},
                        {2, 0},
                        {3, 0},
                        {4, 0},
                        {5, 0},
                        {6, 0}
                } 
            
            },
            {2,
                {
                        {1, 0},
                        {2, 0},
                        {3, 0},
                        {4, 0},
                        {5, 0},
                        {6, 0},
                }
            }
    };

    flat_hash_map<int, string> scenario_descriptions = {
            {1, "Mapped: from matching the first and last kmers only."},
            {2, "Unmapped: one of the two terminal kmers not matched."},
            {3, "Unmapped: Both of the terminal kmers not matched & < %50 of kmers unmatched."},
            {4, "Unmapped: Both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1."},
            {5, "Mapped: Partial match and read is trimmed."},
            {6, "Unmapped: There's no single matched kmer."}
    };


    firstQuery(string pe1_fileName, string pe2_fileName, string idx_prefix, int batchSize = 50000) {
        this->index_prefix = idx_prefix;
        this->PE_1_reads_file = pe1_fileName;
        this->PE_2_reads_file = pe2_fileName;
        this->kSize = kSize;
        this->chunk_size = batchSize;

        std::cerr << "Loading index ..." << std::endl;
        this->ckf = colored_kDataFrame::load(index_prefix);
        this->kf = this->ckf->getkDataFrame();
        this->kSize = this->kf->getkSize();
        std::cerr << "kProcessor index loaded successfully ..." << std::endl;
    }

    tuple<string, bool, int, uint64_t> classifyQuery(std::vector<kmer_row> &kmers, int PE);

    void start_query();


};
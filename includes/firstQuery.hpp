#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithms.hpp>
#include <iostream>


class firstQuery {

public:
    string PE_1_reads_file;
    string PE_2_reads_file;
    string index_prefix;
    int kSize, chunk_size;

    kDataFrame *kf;
    colored_kDataFrame *ckf;


    firstQuery(string pe1_fileName, string pe2_fileName, string idx_prefix, int batchSize = 10000) {
        this->index_prefix = idx_prefix;
        this->PE_1_reads_file = pe1_fileName;
        this->PE_2_reads_file = pe2_fileName;
        this->kSize = kSize;
        this->chunk_size = batchSize;
        std::cerr << "Loading index ..." << std::endl;

        this->ckf = colored_kDataFrame::load(index_prefix);
        this->kf = this->ckf->getkDataFrame();
        this->kSize = this->kf->getkSize();
    }


    void start_query();


};
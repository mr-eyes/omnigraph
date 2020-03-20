#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <vector>
#include <stdint.h>
#include <gqf.h>
#include <algorithms.hpp>
#include <batchQuery.hpp>
#include <Utils/kmer.h>
#include <Utils/utils.hpp>
#include <cmath>
#include <map>


using namespace std;

void QUERY1(kmerDecoder * KD, kDataFrame * KF);

int main(int argc, char** argv){

    string index_prefix = "/home/mabuelanin/Desktop/dev-plan/omnigraph/src/idx_cDBG_SRR11015356_k31unitigs/idx_idx_cDBG_SRR11015356_k31unitigs";
    string PE_1_reads_file = "/home/mabuelanin/Desktop/dev-plan/omnigraph/src/data/SRR11015356_1.fasta";
    string PE_2_reads_file = "/home/mabuelanin/Desktop/dev-plan/omnigraph/src/data/SRR11015356_2.fasta";

    int kSize = 31;
    int chunk_size = 100000;
    int no_chunks = 67954363 / chunk_size;

    
    colored_kDataFrame * ckf = colored_kDataFrame::load(index_prefix);
    kDataFrame * kf = ckf->getkDataFrame();
    

    kmerDecoder *READS_KMERS = new Kmers(PE_1_reads_file, chunk_size, kSize);
    
    // Querying

    int Reads_chunks_counter = no_chunks;

        while (!READS_KMERS->end()) {
        READS_KMERS->next_chunk();
        cout << --Reads_chunks_counter << "remain chunks ..." << endl;

        for (const auto &seq : *READS_KMERS->getKmers()) {
                uint64_t color1 = kf->getCount(seq.second.front().hash);
                uint64_t color2 = kf->getCount(seq.second.back().hash);
                if(color1 == color2){
                    continue;
                }
            }
        }
    

    
    return 0;
}

#include "kDataFrame.hpp"
#include "parallel_hashmap/phmap_dump.h"
#include "cstdint"
#include "string"
#include "assert.h"
#include <chrono>

using namespace std;

int main(){

    string filePath = "../data/idx_cDBG_SRR11015356_k31unitigs/idx_idx_cDBG_SRR11015356_k31unitigs.phmap";
    string PE_1_reads_file = "../data/SRR11015356_1.fasta";

    phmap::flat_hash_map<uint64_t, uint64_t> kmers;
        {
        phmap::BinaryInputArchive ar_in(filePath.c_str());
        kmers.load(ar_in);
        }

    cout << "Loaded " << kmers.size() <<  " Kmers .." << endl;

   

    kmerDecoder *KD = new Kmers(PE_1_reads_file, 1, 31);

    int no_of_sequences = 67954363;
    int no_chunks = no_of_sequences / 1;
    int Reads_chunks_counter = 0;

    while (!KD->end()) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        KD->next_chunk();

        for (const auto &seq : *KD->getKmers()) {
            // std::cout << "Read ID: " << seq.first << std::endl;
            for (const auto &kmer : seq.second) {                
                uint64_t color = kmers[kmer.hash];
            }
        }
        
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto milli = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();



        long hr = milli / 3600000;
        milli = milli - 3600000 * hr;
        long min = milli / 60000;
        milli = milli - 60000 * min;
        long sec = milli / 1000;
        milli = milli - 1000 * sec;
        cerr << "processed chunk: (" << ++Reads_chunks_counter << ") / ("<< no_chunks <<") in ";
        cout << min << ":" << sec << ":" << milli << endl;
    }


}
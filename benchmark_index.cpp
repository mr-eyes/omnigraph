#include "kDataFrame.hpp"
#include "colored_kDataFrame.hpp"
#include "parallel_hashmap/phmap_dump.h"
#include "cstdint"
#include "string"
#include "assert.h"
#include <chrono>
#include "HashUtils/hashutil.hpp"

using namespace std;

int main(){

    string filePath = "../data/idx_cDBG_SRR11015356_k31unitigs/idx_idx_cDBG_SRR11015356_k31unitigs.phmap";
    string PE_1_reads_file = "../data/SRR11015356_1.fasta";

    string mqf_index = "../idx_cDBG_SRR11015356_k31unitigs/mqf_idx_idx_cDBG_SRR11015356_k31unitigs";

    



        phmap::flat_hash_map<uint64_t, uint64_t> kmers;
        {
        phmap::BinaryInputArchive ar_in(filePath.c_str());
        kmers.load(ar_in);
        }

    kDataFrame * mqf = kDataFrame::load(mqf_index);


//    colored_kDataFrame * ckf = colored_kDataFrame::load(filePath);
//    kDataFrame * kmers = kDataFrame::load(filePath);

    std::cout << "Loaded " << kmers.size() <<  " Kmers .." << endl;
    int chunk_size = 50000;
    kmerDecoder *KD = new Kmers(PE_1_reads_file, chunk_size, 31);
//    KD->setHashingMode(kmers->KD->hash_mode);

    int no_of_sequences = 67954363;
    int no_chunks = no_of_sequences / chunk_size;
    int Reads_chunks_counter = 0;

    while (!KD->end()) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        KD->next_chunk();
        
        for (const auto &seq : *KD->getKmers()) {
            // std::cout << "Read ID: " << seq.first << std::endl;

            for (const auto &kmer : seq.second) {                

                phmap::flat_hash_map<uint64_t,uint64_t>::const_iterator got = kmers.find(kmer.hash);
                uint64_t phmap_new_color,phmap_old_color, mqf_color;
                
                if ( got == kmers.end() )
                    phmap_new_color = 0;
                else
                    phmap_new_color = got->second;

                phmap_old_color = kmers[kmer.hash];
                mqf_color = mqf->getCount(kmer.str);

                if(mqf_color != phmap_new_color){
                    cout << kmer.str << ":" << "mqf(" << mqf_color << "), new_phmap( " << phmap_new_color << ")" << endl;
                }

                if(phmap_new_color != phmap_old_color){
                    cout << kmer.str << ":" << "old_phmap(" << phmap_old_color << "), new_phmap( " << phmap_new_color << ")" << endl;
                }

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
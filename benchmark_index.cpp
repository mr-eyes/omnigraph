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

/*
    std::cout << "Iterations" << endl;

    uint64_t mqfKmersCount = 0;

    auto mqf_it = mqf->begin();
    while(mqf_it != mqf->end()){
        mqfKmersCount++;
        mqf_it++;
    }

   std::cout << "mqf.size() = " << mqf->size() << ", actual_kmers = " << mqfKmersCount << endl;

    uint64_t mqf_phmap_mismatches = 0;

    for(const auto & phmap_it : kmers){
        uint64_t kmerHash = phmap_it.first;
        uint64_t phmap_kmerColor = phmap_it.second;
        uint64_t mqf_kmerColor = mqf->getCount(kmerHash);

        if(phmap_kmerColor != mqf_kmerColor) mqf_phmap_mismatches++;
        
    }
    
   std::cout << "MQF & PHMAP Mismatches = " << mqf_phmap_mismatches << endl;

*/


    int chunk_size = 50000;
    kmerDecoder *KD = new Kmers(PE_1_reads_file, chunk_size, 31);
    KD->setHashingMode(1);

    int no_of_sequences = 67954363;
    int no_chunks = no_of_sequences / chunk_size;
    int Reads_chunks_counter = 0;
    

   std::cout << "\n\n-----------------------------------\n Querying the SRR Reads\n" << endl; 

    uint64_t phmap_old_new_mismatches = 0;
    uint64_t mqf_newPhmap_mismatches = 0;
    uint64_t mqf_oldPhmap_mismatches = 0;

   
    while (!KD->end()) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        KD->next_chunk();
        
        for (const auto &seq : *KD->getKmers()) {
            // std::cout << "Read ID: " << seq.first << std::endl;

            for (const auto &kmer : seq.second) {
                uint64_t phmap_new_color,phmap_old_color, mqf_color;

                phmap::flat_hash_map<uint64_t,uint64_t>::const_iterator got = kmers.find(kmer.hash);

                phmap_old_color = kmers[kmer.hash];
                mqf_color = mqf->getCount(kmer.hash);


                if ( got == kmers.end() )
                    phmap_new_color = 0;
                else
                    phmap_new_color = got->second;

                

                if(mqf_color != phmap_new_color){
                    mqf_newPhmap_mismatches++;
                    std::cout << "kmer: " << kmer.str
                            << " | phmap_new_color :" << phmap_new_color
                            << " | phmap_old_color :" << phmap_old_color
                            << " | mqf_color :" << mqf_color << std::endl;

                }

                if(mqf_color != phmap_old_color){
                    mqf_oldPhmap_mismatches++;
                }

                if(phmap_new_color != phmap_old_color){
                    phmap_old_new_mismatches++;
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
        std::cerr << "processed chunk: (" << ++Reads_chunks_counter << ") / ("<< no_chunks <<") in ";
        std::cerr << min << ":" << sec << ":" << milli << endl;
    }


    std::cout << "Query MQF & new_PHMAP Mismatches = " << mqf_newPhmap_mismatches << endl;
    std::cout << "Query MQF & old_PHMAP Mismatches = " << mqf_oldPhmap_mismatches << endl;
    std::cout << "Query old_PHMAP & new_PHMAP Mismatches = " << phmap_old_new_mismatches << endl;


}
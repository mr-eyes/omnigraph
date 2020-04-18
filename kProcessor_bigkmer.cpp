#include <parallel_hashmap/phmap.h>
#include <string>
#include <iostream>
#include <cstdint>
#include <kDataFrame.hpp>
#include <algorithms.hpp>
#include <colored_kDataFrame.hpp>
#include <algorithm>

using namespace std;


int main(int argc, char **argv) {


    string filename = argv[1];
    string names_file = filename + ".names";
    int chunk_size = 10000;
    int kSize = 75;
    int hashing_mode = 3;


    kDataFrame *KF = new kDataFramePHMAP(75, hashing_mode);
    colored_kDataFrame *ckf = kProcessor::index(KF, {{"mode", 1}}, filename, chunk_size, names_file);

    flat_hash_map<size_t, uint16_t> map;

    uint64_t original_kmers_no = 0;

//    kmerDecoder *KD = new Kmers(filename, chunk_size, kSize);
//    KD->setHashingMode(hashing_mode); // Setting hashing mode to bigKmerHasher
//    int counter = 0;
//    while (!KD->end()) {
//        KD->next_chunk();
//        int seqCounter = 0;
//        for (const auto &seq : *KD->getKmers()) {
//            cout << "ID: " << seq.first << endl;
//            for (const auto &kmer : seq.second) {
//                original_kmers_no++;
//            }
//        }
//    }

    original_kmers_no = 258719103;
    auto inserted_kmers = (uint64_t) KF->size();
    cout << "Original kmers: " << original_kmers_no << endl;
    cout << "Inserted kmers: " << inserted_kmers << endl;
    cout << "Lost kmers: " << original_kmers_no - inserted_kmers << endl;
    cout << "_______________________________________________" << endl;

}
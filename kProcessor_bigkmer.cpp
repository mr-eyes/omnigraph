#include <parallel_hashmap/phmap.h>
#include <string>
#include <iostream>
#include <cstdint>
#include <kDataFrame.hpp>
#include <algorithms.hpp>
#include <colored_kDataFrame.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <progressbar.hpp>

using namespace std;

void parse_namesFile(const string &names_fileName, flat_hash_map<uint32_t, uint32_t> &groupNameMap) {
    ifstream namesFile(names_fileName.c_str());
    uint32_t componentID, unitigID;
    string line;
    while (std::getline(namesFile, line)) {
        std::vector<string> tokens;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, '\t'))   // but we can specify a different one
            tokens.push_back(token);
        unitigID = std::stoi(tokens[0].substr(0, tokens[0].find(' ')));
        componentID = std::stoi(tokens[1]);
        groupNameMap[unitigID] = componentID;
    }
}

int main(int argc, char **argv) {


    string filename = argv[1];
    string names_file = filename + ".names";
    int chunk_size = 10000;
    int kSize = 75;
    int hashing_mode = 3;

    flat_hash_map<uint32_t, uint32_t> seq_to_group;
    cout << "Parsing names file ..." << endl;
    parse_namesFile(names_file, seq_to_group);
    int number_of_sequences = seq_to_group.size();
    cout << "Indexing " << number_of_sequences << " seqs ..." << endl;
    progressbar bar(number_of_sequences);
    bar.set_done_char("█");


    kDataFrame *KF = new kDataFramePHMAP(kSize, hashing_mode);


    uint64_t original_kmers_no = 0;

    kmerDecoder *KD = new Kmers(filename, chunk_size, kSize);
    KD->setHashingMode(hashing_mode); // Setting hashing mode to bigKmerHasher
    while (!KD->end()) {
        KD->next_chunk();
        for (const auto &seq : *KD->getKmers()) {
            bar.update();
            for (const auto &kmer : seq.second) {
                original_kmers_no++;
                KF->setCount(kmer.hash, seq_to_group[std::stoi(seq.first.substr(0, seq.first.find(' ')))]);
            }
        }
    }
    cout << endl;


    // Testing query ----------------------------------------------------------------
//    KD = new Kmers(filename, chunk_size, kSize);
//    KD->setHashingMode(hashing_mode); // Setting hashing mode to bigKmerHasher
//    while (!KD->end()) {
//        KD->next_chunk();
//        for (const auto &seq : *KD->getKmers()) {
//            cout << "--------------------------------------------\n";
//            cout << "ID: " << seq.first << endl;
//            for (const auto &kmer : seq.second) {
//                original_kmers_no++;
//                cout << kmer.str << " : " << KF->getCount(kmer.hash) << endl;
//            }
//            cout << "--------------------------------------------\n";
//        }
//    }
//    cout << endl;
    // Testing query ----------------------------------------------------------------

//    original_kmers_no = 258719103;
    auto inserted_kmers = (uint64_t) KF->size();
    cout << "Original kmers: " << original_kmers_no << endl;
    cout << "Inserted kmers: " << inserted_kmers << endl;
    cout << "Lost kmers: " << original_kmers_no - inserted_kmers << endl;
    cout << "_______________________________________________" << endl;

}
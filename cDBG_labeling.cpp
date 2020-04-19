#include <parallel_hashmap/phmap.h>
#include <string>
#include <iostream>
#include <cstdint>
#include <kDataFrame.hpp>
#include <algorithms.hpp>
#include <algorithm>
#include <progressbar.hpp>

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

    if (argc < 4) {
        cerr << "./cDBG_labeling <fasta> <names> <output_prefix>" << endl;
        exit(1);
    }

    const string fasta_file = argv[1];
    const string names_tsv = argv[2];
    const string output_prefix = argv[3];

    int kSize = 75;
    int chunkSize = 100;
    int hashing_mode = 3;

    flat_hash_map<uint32_t, uint32_t> unitig_to_component;

    parse_namesFile(names_tsv, unitig_to_component);

    auto *cDBG = new kDataFramePHMAP(75, hashing_mode);
    kProcessor::kmerDecoder_setHashing(cDBG, hashing_mode);

    int total_seqs = unitig_to_component.size();
    int total_chunks = ceil((double) total_seqs / (double) chunkSize);

    cerr << "total_seqs : " << total_seqs << endl;
    cerr << "chunkSize: " << chunkSize << endl;
    cerr << "total chunks: " << total_chunks << endl;

    progressbar bar(total_chunks);
    bar.set_done_char("█");

    kmerDecoder *KD = new Kmers(fasta_file, chunkSize, kSize);
    KD->setHashingMode(hashing_mode);
    int original_inserted_kmers = 0;
    while (!KD->end()) {
        KD->next_chunk();
        bar.update();
        for (const auto &seq : *KD->getKmers()) {
            for (const auto &kmer : seq.second) {
                uint32_t unitig_id = std::stoi(seq.first.substr(0, seq.first.find(' ')));
                original_inserted_kmers++;
                cDBG->setCount(kmer.hash, unitig_to_component[unitig_id]);
            }
        }
    }
    cout << endl << endl;


    cout << "number of lost kmers: original(" << original_inserted_kmers << ") - inserted(" << cDBG->size()
         << ") = "
         << original_inserted_kmers - cDBG->size() << endl;

    cerr << "saving to disk ...: " << endl;
    cDBG->save(output_prefix);


    /* // Debugging
    cerr << "Test loading: " << endl;
    auto * loaded_cDBG = kDataFrame::load(output_prefix);
    string test_kmer = "TTGTTCTTTTCCAAGGTTCTTGAAGAGTTCTGTTGCTCTAAAATACGCGGTTGAGCTTCCCAGTTTGCCTTTTCT";
    string test_kmer_revComp = "AGAAAAGGCAAACTGGGAAGCTCAACCGCGTATTTTAGAGCAACAGAACTCTTCAAGAACCTTGGAAAAGAACAA";

    cerr << "test_kmer: " << loaded_cDBG->getCount(test_kmer) << endl;
    cerr << "test_kmer: " << loaded_cDBG->getCount(test_kmer_revComp) << endl;
    */



    return 0;
}
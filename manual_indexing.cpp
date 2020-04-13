#include <parallel_hashmap/phmap.h>
#include <string>
#include <iostream>
#include <cstdint>
#include "kmerDecoder.hpp"
#include <algorithm>

using phmap::flat_hash_map;
using namespace std;

string gen_compressed_str(string str)
{
    int len = str.length();
    string result;
    for (int i = 0; i < len; i++) {

        // Count occurrences of current character
        int count = 1;
        while (i < len - 1 && str[i] == str[i + 1]) {
            count++;
            i++;
        }
        // Print character and its count
        if (count == 1)
        {
            result.push_back(str[i]);
        }
        else
        {
            result.push_back(str[i]);
            result.append(to_string(count));
        }

    }
    return result;
}

int main(int argc, char ** argv) {


    string filename = argv[1];
    int chunk_size = 10000;

    flat_hash_map<size_t, uint16_t> map;

    const size_t kSize = 75;

    hash<string> hasher;

    uint64_t original_kmers_no = 0;

    kmerDecoder *KD = new Kmers(filename, chunk_size, kSize);
    int counter = 0;
    while (!KD->end()) {
        KD->next_chunk();
        cout << "Chunk " << ++counter << " ..." << endl;
        int seqCounter = 0;
        for (const auto &seq : *KD->getKmers()) {
            for (const auto &kmer : seq.second) {
                original_kmers_no++;
                map[hasher(kmer.str)] = counter + ++seqCounter;
            }
        }
    }

    uint64_t inserted_kmers = (uint64_t)map.size();
    cout << "Original kmers: " << original_kmers_no << endl;
    cout << "Inserted kmers: " << inserted_kmers << endl;
    cout << "Lost kmers: " << original_kmers_no - inserted_kmers << endl;
    cout << "_______________________________________________" << endl;

}
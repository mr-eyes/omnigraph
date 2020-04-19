#include <iostream>
#include <kDataFrame.hpp>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <algorithms.hpp>
#include <iostream>
#include <tuple>
#include "sqliteManager.hpp"


class Omnigraph {

public:
    int kSize{}, chunk_size{};
    SQLiteManager *SQL{};
    vector<kmerDecoder *> READS_DECODER;
    string PE_1_reads_file, PE_2_reads_file;

    flat_hash_map<int, flat_hash_map<int, int>> scenarios_count =
            {
                    {1,
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
            {2, "Unmapped: Both terminal kmers matched but on different components."},
            {3, "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched."},
            {4, "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1."},
            {5, "Mapped: Partial match and read is trimmed."},
            {6, "Unmapped: There's no single matched kmer."}
    };

    Omnigraph() {
    }

    tuple<string, bool, int, uint64_t> classifyRead(kDataFrame *kf, std::vector<kmer_row> &kmers, int PE);
    tuple<string, bool, int, uint32_t, double> classifyRead_withStats(kDataFrame *kf, std::vector<kmer_row> &kmers, int PE);

    static string kmers_to_seq(vector<kmer_row> &kmers);
};
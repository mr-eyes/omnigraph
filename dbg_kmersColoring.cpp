#include <coloredKmers.hpp>
#include <progressbar.hpp>

int main(int argc, char **argv) {

    if (argc < 4) {
        cerr << "./kmersColoring <fasta> <names> <output_dir>" << endl;
        exit(1);
    }

    const string fasta_file = argv[1];
    const string names_tsv = argv[2];
    const string output_dir = argv[3];
    int kSize = 75;
    int chunkSize = 100;
    int hashing_mode = 3;

    auto *cDBG = new ColoredKmers(names_tsv);
    int total_seqs = cDBG->get_seqs_number();
    int total_chunks = ceil((double) total_seqs / (double) chunkSize);

    cerr << "total_seqs : " << total_seqs << endl;
    cerr << "chunkSize: " << chunkSize << endl;
    cerr << "total chunks: " << total_chunks << endl;

    progressbar bar(total_chunks);
    bar.set_done_char("█");

    kmerDecoder *KD = new Kmers(fasta_file, chunkSize, kSize);
    KD->setHashingMode(hashing_mode);

    while (!KD->end()) {
        KD->next_chunk();
        bar.update();
        for (const auto &seq : *KD->getKmers()) {
            for (const auto &kmer : seq.second) {
                uint32_t unitig_id = std::stoi(seq.first.substr(0, seq.first.find(' ')));
                cDBG->setKmerColor(kmer.hash, unitig_id);
            }
        }
    }
    cout << endl << endl;


    cout << "number of lost kmers: original(" << cDBG->original_inserted_kmers << ") - inserted(" << cDBG->size()
         << ") = "
         << cDBG->original_inserted_kmers - cDBG->size() << endl;

    cerr << "saving to disk ...: " << endl;
    string returned_out_dir = cDBG->save(output_dir);
    cerr << "saved in : " << returned_out_dir << endl;


    /*
    cerr << "Test loading: " << endl;
    auto * loaded_cDBG = ColoredKmers::load(returned_out_dir);
    string test_kmer = "TTGTTCTTTTCCAAGGTTCTTGAAGAGTTCTGTTGCTCTAAAATACGCGGTTGAGCTTCCCAGTTTGCCTTTTCT";
    string test_kmer_revComp = "AGAAAAGGCAAACTGGGAAGCTCAACCGCGTATTTTAGAGCAACAGAACTCTTCAAGAACCTTGGAAAAGAACAA";

    cerr << "test_kmer: " << loaded_cDBG->getKmerColor(test_kmer) << endl;
    cerr << "test_kmer: " << loaded_cDBG->getKmerColor(test_kmer_revComp) << endl;
    */



    return 0;
}
#include <coloredKmers.hpp>
#include <progressbar.hpp>

int main(int argc, char **argv) {

    if (argc < 3) {
        cerr << "./kmersColoring <fasta> <names>" << endl;
        exit(1);
    }

    const string fasta_file = argv[1];
    const string names_tsv = argv[2];
    int kSize = 75;
    int chunkSize = 100;
    int hashing_mode = 3;

    auto *cDBG = new ColoredKmers(names_tsv, kSize, hashing_mode);
    int total_seqs = cDBG->get_seqs_number();
    int total_chunks = ceil((double)total_seqs / (double)chunkSize);

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
    cout << endl;


    cout << "number of lost kmers: original(" << cDBG->original_inserted_kmers << ") - inserted(" << cDBG->size() << ") = "
         << cDBG->original_inserted_kmers - cDBG->size() << endl;


    return 0;
}
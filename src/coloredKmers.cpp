#include <coloredKmers.hpp>
#include "parallel_hashmap/phmap_dump.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;


uint32_t ColoredKmers::getKmerColor(const string &kmer_str) {
    phmap::flat_hash_map<uint64_t, uint32_t>::const_iterator got = this->kmers.find(KD->hash_kmer(kmer_str));
    if (got == this->kmers.end())
        return 0;
    else
        return got->second;
    return 0;
}

ColoredKmers *ColoredKmers::load(const string &prefix) {
    string out_dir = prefix;
    const string metadata_file = out_dir + '/' + prefix + "_metadata.omni";
    const string kmers_file = out_dir + '/' + prefix + "_kmers.omni";
//    const string names_file = out_dir + '/' + prefix + "_names.omni";

    auto *cKmer = new ColoredKmers();
    {
        phmap::BinaryInputArchive metadata_in(metadata_file.c_str());
        cKmer->metadata.load(metadata_in);
        phmap::BinaryInputArchive kmers_in(kmers_file.c_str());
        cKmer->kmers.load(kmers_in);

    }

    cKmer->KD = new Kmers(cKmer->metadata['k'], cKmer->metadata['h']);

    return cKmer;
}

inline string create_dir(const string &output_file, int serial) {
    int dir_err;
    string new_name = "";

    if (!serial) {
        dir_err = mkdir(output_file.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        new_name = output_file;
    } else {
        new_name = output_file + "_v." + to_string(serial);
        dir_err = mkdir(new_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    if (-1 == dir_err) return create_dir(output_file, ++serial);

    return new_name;
}

void *ColoredKmers::save(const string &prefix) {

    string out_dir = create_dir(prefix, 0);
    const string metadata_file = out_dir + '/' + prefix + "_metadata.omni";
    const string kmers_file = out_dir + '/' + prefix + "_kmers.omni";
//    const string names_file = out_dir + '/' + prefix + "_names.omni";

    {
        phmap::BinaryOutputArchive metadata_out(metadata_file.c_str());
        this->kmers.dump(metadata_out);
    }

    {
        phmap::BinaryOutputArchive kmers_out(kmers_file.c_str());
        this->kmers.dump(kmers_out);
    }
}

ColoredKmers::ColoredKmers(const string &namesToGroupsFile, int kSize, int hashing_mode = 3) {
    this->metadata['h'] = hashing_mode;
    this->metadata['k'] = kSize;
    this->KD = new Kmers(kSize, hashing_mode);
    KD->setHashingMode(hashing_mode);
    // Loading names TSV file
    this->load_names(namesToGroupsFile);
    this->original_inserted_kmers = 0;
}


void ColoredKmers::load_names(const string &names_tsv_path) {
    ifstream namesFile(names_tsv_path.c_str());
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
        this->unitigToComponent[unitigID] = componentID;
    }
}

void ColoredKmers::setKmerColor(uint64_t kmer_hash, uint32_t &unitig_id) {
    this->original_inserted_kmers++;
    this->kmers[kmer_hash] = this->unitigToComponent[unitig_id];
}

int ColoredKmers::size() {
    return this->kmers.size();
}

void ColoredKmers::setKmerColor(string & kmer_str, uint32_t & unitig_id) {
    this->original_inserted_kmers++;
    this->kmers[this->KD->hash_kmer(kmer_str)] = this->unitigToComponent[unitig_id];
}

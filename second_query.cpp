#include "sqliteManager.hpp"
#include "string"
#include "kDataFrame.hpp"
#include "INIReader.h"
#include <glob.h>
#include <sstream>
#include <stdexcept>
#include "omnigraph.hpp"
#include "tuple"

using namespace std;

string getFileName(const string &s) {

    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = s.rfind(sep, s.length());
    if (i != string::npos) {
        return (s.substr(i + 1, s.length() - i));
    }

    return ("");
}


std::vector<std::string> glob(const std::string &pattern) {
    using namespace std;

    // glob struct resides on the stack
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

    // do the glob operation
    int return_value = glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    if (return_value != 0) {
        globfree(&glob_result);
        stringstream ss;
        ss << "glob() failed with return_value " << return_value << endl;
        throw std::runtime_error(ss.str());
    }

    // collect all the filenames into a std::list<std::string>
    vector<string> filenames;
    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        filenames.push_back(string(glob_result.gl_pathv[i]));
    }

    // cleanup
    globfree(&glob_result);

    // done
    return filenames;
}


const string prepare_update(int ID, int seq1_original_component, int seq2_original_component) {
    const string _sqlite_update = "UPDATE reads SET "
                                  "seq1_original_component=" + to_string(seq1_original_component)
                                  + ", seq2_original_component=" + to_string(seq2_original_component)
                                  + " WHERE ID=" + to_string(ID) + ";";

    return _sqlite_update;
}

int main() {

    string config_file_path = "../config.ini";
    string index_prefix, PE_1_reads_file, PE_2_reads_file, sqlite_db, collective_comps_indexes_dir;
    int batchSize, kSize, no_of_sequences;

    INIReader reader(config_file_path);

    if (reader.ParseError() != 0) {
        std::cout << "Can't load '" << config_file_path << "'\n";
        return 1;
    }


    index_prefix = reader.Get("kProcessor", "idx_prefix", "");
    collective_comps_indexes_dir = reader.Get("kProcessor", "collective_comps_indexes_dir", "");
    PE_1_reads_file = reader.Get("Reads", "read1", "");
    PE_2_reads_file = reader.Get("Reads", "read2", "");
    no_of_sequences = reader.GetInteger("Reads", "seqs_no", 0);
    sqlite_db = reader.Get("SQLite", "db_file", "query1_result.db");
    batchSize = reader.GetInteger("kProcessor", "chunk_size", 1);
    kSize = reader.GetInteger("kProcessor", "ksize", 31);

    cerr << "Fetch kProcessor indexes paths ..." << endl;

    std::vector<std::string> filenames = glob(collective_comps_indexes_dir);
    std::map<int, string> index_paths;


    for (auto &filename : filenames) {
        string _base_name, _index_prefix;
        _base_name = getFileName(filename);
        int idx_no = std::stoi(_base_name.substr(4, 3));
        _index_prefix.append(filename + "/" + _base_name);
        index_paths[idx_no] = _index_prefix;
    }

    kDataFrame *kf;
    Omnigraph *second_query = new Omnigraph();
    kmerDecoder *KD = new Kmers(kSize);
    std::vector<kmer_row> kmers;

    string db_file;
    auto *SQL = new SQLiteManager(sqlite_db);

    // Make it faster
    string _sqlite_sync = "PRAGMA synchronous = OFF;";
    SQL->rc = sqlite3_exec(SQL->db.db_, _sqlite_sync.c_str(), SQL->callback, 0, &SQL->zErrMsg);

    std::map<int, string> queries = {
            {1, "SELECT ID, PE_seq1 FROM reads WHERE seq1_collective_component="},
            {2, "SELECT ID, PE_seq2 FROM reads WHERE seq2_collective_component="},
    };

    const string _sqlite_update = "UPDATE reads SET "
                                  "seq1_original_component=" + to_string(1)
                                  + " WHERE ID=" + to_string(1) + ";";

    for (const auto &idx : index_paths) {
        kf = kDataFrame::load(idx.second);

        cerr << "Processing collective component (" << idx.first << ") ..." << endl;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        for (int PEcollectiveComponents = 1; PEcollectiveComponents <= 2; PEcollectiveComponents++) {
            const string _sqlite_select = queries[PEcollectiveComponents] + std::to_string(idx.first) + ";";

            sqlite3pp::query qry(SQL->db, _sqlite_select.c_str());

            // Iterate over the database reads
            for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
                int ID;
                const char *PE_seq;

                std::tie(ID, PE_seq) = (*i).get_columns < int, char const* > (0, 1);

                string seq = PE_seq;
                KD->seq_to_kmers(seq, kmers);
                tuple<string, bool, int, uint64_t> read_result = second_query->classifyRead(kf, kmers,
                                                                                            PEcollectiveComponents);

                string read_1_constructedRead = get<0>(read_result);
                bool read_1_mapped_flag = get<1>(read_result);
                int seq_original_component = get<2>(read_result);

                const string _sqlite_update = "UPDATE reads SET "
                                              "seq" + to_string(PEcollectiveComponents) + "_original_component=" +
                                              to_string(seq_original_component)
                                              + " WHERE ID=" + to_string(ID) + ";";


                SQL->rc = sqlite3_exec(SQL->db.db_, _sqlite_update.c_str(), SQL->callback, 0, &SQL->zErrMsg);
            }


        }

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        long hr = milli / 3600000;
        milli = milli - 3600000 * hr;
        long min = milli / 60000;
        milli = milli - 60000 * min;
        long sec = milli / 1000;
        milli = milli - 1000 * sec;
        cerr << "Processing collective components: (" << idx.first << ") in ";
        cout << min << ":" << sec << ":" << milli << endl;

        delete kf;
    }

    delete KD;
    SQL->close();


    return 0;
}
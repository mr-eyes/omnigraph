#include "omnigraph.hpp"

tuple<string, bool, int, uint64_t>
Omnigraph::classifyRead(kDataFrame *kf, std::vector<kmer_row> &kmers, int PE) {

    /*
     * scenarios:
        1 "Mapped: from matching the first and last kmers only."
        2 "Unmapped: Both terminal kmers matched but on different components."
        3 "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched."
        4 "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1."
        5 "Mapped: Partial match and read is trimmed."
        6 "Unmapped: There's no single matched kmer."
     * */

    string constructed_read;
    int scenario = 0;
    int connected_component;
    uint64_t color1 = kf->getCount(kmers.front().hash);
    uint64_t color2 = kf->getCount(kmers.back().hash);

    if (color1 != 0 && color2 != 0) {

        if (color1 == color2) {
            constructed_read = kmers_to_seq(kmers);
            scenario = 1;
            this->scenarios_count[PE][scenario]++;
            connected_component = color1;
            return make_tuple(constructed_read, true, scenario, connected_component);

        } else {
            scenario = 2;
            this->scenarios_count[PE][scenario]++;
            constructed_read = kmers_to_seq(kmers);
            return make_tuple(constructed_read, false, scenario, 0);
        }
    } else {
        // That mean both are zeros so both could not be found
        // Case 3
        double noKmers = (double) kmers.size();

        vector<uint64_t> all_colors;

        phmap::flat_hash_set<uint64_t> unique_colors;
        double found_count = 0;


        // Get all the colors
        for (const auto &kmer: kmers) {
            uint64_t color = kf->getCount(kmer.hash); // This line is a landmine, take care.
            if (color != 0) {
                found_count++;
            }
            all_colors.push_back(color);
            unique_colors.insert(color);
        }


        // Check found Vs. unfound
        if ((found_count / noKmers) < 0.5) {
            // not aligned read

            scenario = 3;
            this->scenarios_count[PE][scenario]++;
            constructed_read = kmers_to_seq(kmers);
            return make_tuple(constructed_read, false, scenario, 0);

        } else {

            if (unique_colors.size() > 2) {
                // unfound = 0, found = !0, so if all the kmers are coming from single component then the unique number of colors should be 2

                scenario = 4;
                this->scenarios_count[PE][scenario]++;
                constructed_read = kmers_to_seq(kmers);
                return make_tuple(constructed_read, false, scenario, 0);
            } else if (unique_colors.size() == 2) { // This is important, to assure there's an exact one color found.

                int start_kmer, end_kmer;
                start_kmer = 0;
                end_kmer = 0;
                auto it = all_colors.begin();
                uint64_t collectiveComponent;

                for (auto _color : unique_colors) {
                    if (_color != 0) {
                        collectiveComponent = _color;
                        break;
                    }
                }

                while (it != all_colors.end()) {
                    if (*it != 0) {
                        // cout << "*it = " << *it << endl;
                        start_kmer = distance(all_colors.begin(), it);
                        // cout << "start_kmer = " << start_kmer << endl;
                        auto rev_it = all_colors.end();
                        while (rev_it != it) {
                            rev_it--;
                            if (*rev_it != 0) {
                                end_kmer = distance(all_colors.begin(), rev_it);
                                it = all_colors.end();
                                break;
                            }
                        }
                    } else {
                        it++;
                    }
                }

                scenario = 5;
                this->scenarios_count[PE][scenario]++;

                vector<kmer_row> sliced_kmers = std::vector<kmer_row>(kmers.begin() + start_kmer,
                                                                      kmers.begin() + end_kmer + 1);
                constructed_read = kmers_to_seq(sliced_kmers);

                return make_tuple(constructed_read, true, scenario, collectiveComponent);

            } else {
                scenario = 6;
                this->scenarios_count[PE][scenario]++;
                constructed_read = kmers_to_seq(kmers);
                return make_tuple(constructed_read, false, scenario, 0);
            }
        }
    }

}

string Omnigraph::kmers_to_seq(vector<kmer_row> &kmers) {
    string seq;
    int kSize = kmers[0].str.size();
    int i;
    for (i = 0; i < (int) kmers.size(); i += kSize)
        seq += kmers[i].str;

    return seq.substr(0, (kmers.size() - 1)) + kmers.back().str;
}

// tuple<seq, matched, scenario, component, found_ratio>
tuple<string, bool, int, uint32_t, double>
Omnigraph::classifyRead_withStats(kDataFrame *kf, std::vector<kmer_row> &kmers, int PE) {


    /*
     * scenarios:
        1 "Mapped: from matching the first and last kmers only."
        2 "Unmapped: Both terminal kmers matched but on different components."
        3 "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched."
        4 "Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1."
        5 "Mapped: Partial match and read is trimmed."
        6 "Unmapped: There's no single matched kmer."
     * */

    string constructed_read;
    int scenario = 0;
    int connected_component;
    double noKmers = (double) kmers.size();

    vector<uint64_t> all_colors;
    phmap::flat_hash_set<uint64_t> unique_colors;
    double found_count = 0;

    // Get all the colors
    for (const auto &kmer: kmers) {
        uint64_t color = kf->getCount(kmer.hash);
        if (color != 0) {
            found_count++;
        }
        all_colors.push_back(color);
        unique_colors.insert(color);
    }

    double found_ratio = found_count / noKmers;

    uint64_t color1 = all_colors.front();
    uint64_t color2 = all_colors.back();

    if (color1 != 0 && color2 != 0) {

        if (color1 == color2) {
            constructed_read = kmers_to_seq(kmers);
            scenario = 1;
            this->scenarios_count[PE][scenario]++;
            connected_component = color1;
            return make_tuple(constructed_read, true, scenario, connected_component, found_ratio);

        } else {
            scenario = 2;
            this->scenarios_count[PE][scenario]++;
            constructed_read = kmers_to_seq(kmers);
            return make_tuple(constructed_read, false, scenario, 0, found_ratio);
        }
    } else {
        // That mean both are zeros so both could not be found
        // Case 3



        // Check found Vs. unfound
        if (found_ratio < 0.5) {
            // not aligned read

            scenario = 3;
            this->scenarios_count[PE][scenario]++;
            constructed_read = kmers_to_seq(kmers);
            return make_tuple(constructed_read, false, scenario, 0, found_ratio);

        } else {

            if (unique_colors.size() > 2) {
                // unfound = 0, found = !0, so if all the kmers are coming from single component then the unique number of colors should be 2

                scenario = 4;
                this->scenarios_count[PE][scenario]++;
                constructed_read = kmers_to_seq(kmers);
                return make_tuple(constructed_read, false, scenario, 0, found_ratio);
            } else if (unique_colors.size() == 2) { // This is important, to assure there's an exact one color found.

                int start_kmer, end_kmer;
                start_kmer = 0;
                end_kmer = 0;
                auto it = all_colors.begin();
                uint32_t connectedComponent;

                for (auto _color : unique_colors) {
                    if (_color != 0) {
                        connectedComponent = _color;
                        break;
                    }
                }

                while (it != all_colors.end()) {
                    if (*it != 0) {
                        // cout << "*it = " << *it << endl;
                        start_kmer = distance(all_colors.begin(), it);
                        // cout << "start_kmer = " << start_kmer << endl;
                        auto rev_it = all_colors.end();
                        while (rev_it != it) {
                            rev_it--;
                            if (*rev_it != 0) {
                                end_kmer = distance(all_colors.begin(), rev_it);
                                it = all_colors.end();
                                break;
                            }
                        }
                    } else {
                        it++;
                    }
                }

                scenario = 5;
                this->scenarios_count[PE][scenario]++;

                vector<kmer_row> sliced_kmers = std::vector<kmer_row>(kmers.begin() + start_kmer,
                                                                      kmers.begin() + end_kmer + 1);
                constructed_read = kmers_to_seq(sliced_kmers);

                return make_tuple(constructed_read, true, scenario, connectedComponent, found_ratio);

            } else {
                scenario = 6;
                this->scenarios_count[PE][scenario]++;
                constructed_read = kmers_to_seq(kmers);
                return make_tuple(constructed_read, false, scenario, 0, found_ratio);
            }
        }
    }

}
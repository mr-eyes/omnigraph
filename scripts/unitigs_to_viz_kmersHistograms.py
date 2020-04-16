"""
This script plots histogram of KC, KM, LN fields in unitigs.fa file
"""

import os
import subprocess
import sys

import plotly.graph_objs as go
from plotly.offline import plot
from tqdm import tqdm

"""
# TODO

LN: print
    - Frequency of unitigs with length == 75
    - Sum of frequencies of unitigs length [76:80]
    - Sum of frequencies of unitigs length [81:151]
    - Sum of frequencies of unitigs length >151
    - Maximum found length

KM: print
    - Frequency of unitigs with KM [1.0: <= 2.0]
    - Frequency of unitigs with KM [> 2.0: <= 10.0]
    - Frequency of unitigs with KM [> 10.0:<= 50.0]
    - Frequency of unitigs with KM [> 50.0:<= 100.0]
    - Frequency of unitigs with KM [> 100.0:<= 1000.0]
    - Frequency of unitigs with KM [> 1000.0:<= 10000.0]
    - Frequency of unitigs with KM [> 10000.0]
"""


class Stats:
    km_stats = dict()
    km_max = dict()
    ln_stats = dict()
    ln_prints = {"==75": "Frequency of unitigs with length == 75",
                 "76:80": "Sum of frequencies of unitigs length [76:80]",
                 "81:151": "Sum of frequencies of unitigs length [81:151]",
                 ">151": "Sum of frequencies of unitigs length >151"}

    km_prints = {"1:2": "Frequency of unitigs with KM [1.0: <= 2.0]",
                 "2:10": "Frequency of unitigs with KM [> 2.0: <= 10.0]",
                 "10:50": "Frequency of unitigs with KM [> 10.0:<= 50.0]",
                 "50:100": "Frequency of unitigs with KM [> 50.0:<= 100.0]",
                 "100:1000": "Frequency of unitigs with KM [> 100.0:<= 1000.0]",
                 "1000:10000": "Frequency of unitigs with KM [> 1000.0:<= 10000.0]",
                 ">10000": "Frequency of unitigs with KM [> 10000.0]"}

    def __init__(self, number_of_components):
        self.km_max = 0
        self.km_keys = ["==75", "76:80", "81:151", ">151"]
        self.ln_keys = ["1:2", "2:10", "10:50", "50:100", "100:1000", "1000:10000", ">10000"]
        for _compID in range(1, number_of_components + 1, 1):
            self.number_of_components = number_of_components
            self.km_stats[_compID] = dict()
            self.ln_stats[_compID] = dict()
            for k in self.km_keys:
                self.km_stats[_compID][key] = 0
            for k in self.ln_keys:
                self.ln_stats[_compID][key] = 0

    def stats_insert_ln(self, _compID, ln):
        if ln > 10000:
            self.ln_stats[_compID][">10000"] += 1
        elif 1000 < ln <= 10000:
            self.ln_stats[_compID]["1000:10000"] += 1
        elif 50 > ln <= 100:
            self.ln_stats[_compID]["50:100"] += 1
        elif 10 > ln <= 50:
            self.ln_stats[_compID]["10:50"] += 1
        elif 2 > ln <= 10:
            self.ln_stats[_compID]["2:10"] += 1
        elif 1 > ln <= 2:
            self.ln_stats[_compID]["1:2"] += 1

    def stats_insert_km(self, _compID, km):
        if km > self.km_max:
            self.km_max = km
        if km > 151:
            self.km_stats[_compID][">151"] += 1
        elif 81 <= km <= 151:
            self.km_stats[_compID]["81:151"] += 1
        elif 76 <= km <= 80:
            self.km_stats[_compID]["76:80"] += 1
        elif km == 75:
            self.km_stats[_compID]["==75"] += 1

    def get_stats_of_comp(self, _compID):
        print(f"Stats for component {_compID}:\n")
        print(f"LN Field Stats: ")
        for k, v in self.ln_stats[_compID].items():
            print(f"\t - {self.ln_prints[k]} : {v}")

        print(f"\nKM Field Stats: ")
        for k, v in self.km_stats[_compID].items():
            print(f"\t - {self.km_prints[k]} : {v}")

    def get_stats_all_comps(self):
        print(f"Stats for all components: \n")

        tmp_km_stats = dict()
        tmp_ln_stats = dict()
        for _compID in range(1, self.number_of_components + 1, 1):
            for k in self.km_keys:
                tmp_km_stats[key] = 0
            for k in self.ln_keys:
                tmp_ln_stats[key] = 0

            for k, v in self.ln_stats[_compID].items():
                tmp_ln_stats[k] += v

            for k, v in self.km_stats[_compID].items():
                tmp_km_stats[k] += v

        print(f"LN Field Stats: ")
        for k, v in tmp_ln_stats.items():
            print(f"\t - {self.ln_prints[k]} : {v}")

        print(f"\nKM Field Stats: ")
        for k, v in tmp_ln_stats.items():
            print(f"\t - {self.km_prints[k]} : {v}")


if len(sys.argv) < 2:
    sys.exit("run: python unitigs_edges_historgram.py <unitigs_fasta> <components_csv> <_compID>")

unitigs_file = sys.argv[1]
no_seqs = int(subprocess.getoutput('wc -l ' + unitigs_file).split()[0]) // 2

components_csv = sys.argv[2]
components_no = int(subprocess.getoutput('wc -l ' + components_csv).split()[0])

stats = Stats(components_no)

focus_compID = int(sys.argv[3])

print("Parsing Fasta file ...")

KM = dict()
KC = dict()
LN = dict()

unitig_to_size = dict()


def get_nodeID(link):
    return int(link[4:-2])


main_unitigs = list()
unitig_to_component = dict()
component_to_unitigs = dict()
with open(components_csv, 'r') as compsReader:
    for line in compsReader:
        line = list(map(int, line.strip().split(',')))
        compID = line[0]
        component_to_unitigs[compID] = list()
        KM[compID] = list()
        KC[compID] = list()
        LN[compID] = list()
        for unitig in line[1:]:
            unitig_to_component[unitig] = compID
            component_to_unitigs[compID].append(unitig)

with open(unitigs_file, 'r') as unitigsReader:
    for line in tqdm(unitigsReader, total=no_seqs):
        seq_len = next(unitigsReader).strip()
        header = line.strip().split()

        unitig_id = int(header[0][1:])
        compID = unitig_to_component[unitig_id]

        _ln = float(header[1].split(":")[-1])
        LN[compID].append(_ln)
        stats.stats_insert_ln(compID, _ln)

        _kc = float(header[2].split(":")[-1])
        KC[compID].append(_kc)

        _km = float(header[3].split(":")[-1])
        KM[compID].append(_km)
        stats.stats_insert_km(compID, _km)

print("Generating HTML viz...")

figs = {
    "KC": go.Figure(data=[go.Histogram(x=KC[focus_compID])]),
    "KM": go.Figure(data=[go.Histogram(x=KM[focus_compID])]),
    "LN": go.Figure(data=[go.Histogram(x=LN[focus_compID])]),
}

print("\n ------------------------------ \n ")
stats.get_stats_all_comps()
stats.get_stats_of_comp(focus_compID)

for key, fig in figs.items():
    print(f"Saving {key} HTML")
    output_file = ".".join(os.path.basename(unitigs_file).split(".")[:-1]) + f"{key}_histo.html"
    fig.update_layout(barmode='group', )
    plot(fig, filename=output_file, auto_open=False)

    print(f"Saving {key}  y Log HTML")
    output_file = ".".join(os.path.basename(unitigs_file).split(".")[:-1]) + f"{key}_yLog_histo.html"
    fig.update_layout(barmode='group', yaxis_type="log")
    plot(fig, filename=output_file, auto_open=False)

"""
This script plots histogram of KC, KM, LN fields in unitigs.fa file
"""


import os
import subprocess
import sys
import plotly.graph_objs as go
from plotly.offline import plot
from tqdm import tqdm

if len(sys.argv) < 2:
    sys.exit("run: python unitigs_edges_historgram.py <unitigs_fasta> <components_csv> <compID>")

unitigs_file = sys.argv[1]
no_seqs = int(subprocess.getoutput('wc -l ' + unitigs_file).split()[0]) // 2

components_csv = sys.argv[2]
components_no = int(subprocess.getoutput('wc -l ' + components_csv).split()[0])

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
        seq_len = next(unitigsReader)
        header = line.strip().split()

        unitig_id = int(header[0][1:])
        compID = unitig_to_component[unitig_id]

        _ln = float(header[1].split(":")[-1])
        LN[compID].append(_ln)

        _kc = float(header[2].split(":")[-1])
        KC[compID].append(_kc)

        _km = float(header[3].split(":")[-1])
        KM[compID].append(_km)

print("Generating HTML viz...")

figs = {
    "KC": go.Figure(data=[go.Histogram(x=KC[focus_compID])]),
    "KM": go.Figure(data=[go.Histogram(x=KM[focus_compID])]),
    "LN": go.Figure(data=[go.Histogram(x=LN[focus_compID])]),
}

for key, fig in figs.items():
    print(f"Saving {key} HTML")
    output_file = ".".join(os.path.basename(unitigs_file).split(".")[:-1]) + f"{key}_histo.html"
    fig.update_layout(barmode='group', )
    plot(fig, filename=output_file, auto_open=False)

    print(f"Saving {key}  y Log HTML")
    output_file = ".".join(os.path.basename(unitigs_file).split(".")[:-1]) + f"{key}_yLog_histo.html"
    fig.update_layout(barmode='group', yaxis_type="log")
    plot(fig, filename=output_file, auto_open=False)

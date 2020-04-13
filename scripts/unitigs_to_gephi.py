import subprocess
from tqdm import tqdm
import sys
import os

if len(sys.argv) < 2:
    sys.exit("run: python unitigs_edges_historgram.py <unitigs_fasta> <components_csv> <compID>")

unitigs_file = sys.argv[1]
no_seqs = int(subprocess.getoutput('wc -l ' + unitigs_file).split()[0]) // 2

components_csv = sys.argv[2]
components_no = int(subprocess.getoutput('wc -l ' + components_csv).split()[0])

focus_compID = int(sys.argv[3])

print("Parsing Fasta file ...")

source = dict()
target = dict()

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
        source[compID] = list()
        target[compID] = list()

        for unitig in line[1:]:
            unitig_to_component[unitig] = compID
            component_to_unitigs[compID].append(unitig)

with open(unitigs_file, 'r') as unitigsReader:
    for line in tqdm(unitigsReader, total=no_seqs):
        seq_len = len(next(unitigsReader).strip())
        header = line.strip().split()
        unitig_id = int(header[0][1:])
        compID = unitig_to_component[unitig_id]
        links = list(map(get_nodeID, header[4:]))
        for link in links:
            source[compID].append(unitig_id)
            target[compID].append(link)

final_edges = set()
nodes_length = len(source[focus_compID])

print("Constructing edges list ...")

for i in tqdm(range(nodes_length)):
    source_node = source[focus_compID][i]
    target_node = target[focus_compID][i]

    if source_node > target_node:
        final_edges.add((source_node, target_node))
    else:
        final_edges.add((target_node, source_node))

output_file = ".".join(os.path.basename(unitigs_file).split(".")[:-1]) + "_gephi.csv"
print(f"Writing gephi csv: {output_file}")
with open(output_file, 'w') as gephiWriter:
    gephiWriter.write(f"Source,Target\n")

    assert len(source[focus_compID]) == len(target[focus_compID])

    for source_node, target_node in tqdm(final_edges):
        gephiWriter.write(f"{source_node},{target_node}\n")

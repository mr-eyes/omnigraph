"""
This script is used under the project "Omnigraph"

Input:
    1- Fasta file of the cDBG unitigs

Output:
    1- Fixed Fasta file of the cDBG unitigs

Run:
    - python fix_unitigs_edges.py <unitigs_fasta>

Example:
    - python fix_unitigs_edges.py unitigs.fa unitigs.fa.components.tsv

"""

import sys
import os
import subprocess
from tqdm import tqdm


def get_nodeID(link):
    return int(link[4:-2])


if len(sys.argv) < 2:
    sys.exit("run: python fix_unitigs_edges.py <unitigs_fasta>")

unitigs_fasta = sys.argv[1]
no_seqs = int(subprocess.getoutput('wc -l ' + unitigs_fasta).split()[0]) // 2

nodes_set = set()
print(f"Scanning nodes ...", file=sys.stderr)
with open(unitigs_fasta, 'r') as unitigsReader:
    for line in tqdm(unitigsReader, total=no_seqs):
        seq_len = len(next(unitigsReader).strip())
        header = line.strip().split()
        unitig_id = int(header[0][1:])
        nodes_set.add(unitig_id)

print(f"Fixing edges ...", file=sys.stderr)
output_file = "fixed_" + unitigs_fasta
deleted_nodes_count = 0
with open(unitigs_fasta, 'r') as unitigsReader, open(output_file, 'w') as unitigsWriter:
    for line in tqdm(unitigsReader, total=no_seqs):
        seq = next(unitigsReader).strip()
        header = line.strip().split()
        new_header = " ".join(header[:4]) + "  "
        unitig_id = int(header[0][1:])
        links_str_list = header[4:]
        links = set(map(get_nodeID, links_str_list))
        correct_links = links.intersection(nodes_set)

        # Uncomment if you want to delete edgeless nodes
        # if len(correct_links) == 0:
        #     deleted_nodes_count += 1
        #     continue

        for link in links_str_list:
            if get_nodeID(link) in correct_links:
                new_header += " " + link

        unitigsWriter.write(f"{new_header}\n{seq}\n")


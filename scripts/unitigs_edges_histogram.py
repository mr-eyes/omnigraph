import os
import subprocess
from tqdm import tqdm
import sys
from collections import Counter

if len(sys.argv) < 2:
    sys.exit("run: python unitigs_edges_historgram.py <unitigs_fasta> <components_csv>")

unitigs_file = sys.argv[1]
no_seqs = int(subprocess.getoutput('wc -l ' + unitigs_file).split()[0]) // 2

components_csv = sys.argv[2]
components_no = int(subprocess.getoutput('wc -l ' + components_csv).split()[0])

unitig_to_component = dict()
histogram = dict()

with open(components_csv, 'r') as compsReader:
    for line in compsReader:
        line = list(map(int, line.strip().split(',')))
        compID = line[0]
        histogram[compID] = list()
        for unitig in line[1:]:
            unitig_to_component[unitig] = compID

histo_keys = list()
for i in range(-4, 1):
    for j in range(0, 5):
        histo_keys.append((i, j))

with open(unitigs_file, 'r') as unitigsReader:
    for line in tqdm(unitigsReader, total=no_seqs):
        next(unitigsReader)  # skip seq
        header = line.strip().split()
        unitigID = int(header[0][1:])
        all_links = header[4:]
        incoming = sum('L:-' in s for s in all_links) * -1
        outcoming = sum('L:+' in s for s in all_links)
        compID = unitig_to_component[unitigID]
        histogram[compID].append((incoming, outcoming))

histogram_counts = dict()

for compID, edges in histogram.items():
    histogram_counts[compID] = dict(Counter(histogram[compID]))

output_file = ".".join(os.path.basename(unitigs_file).split(".")[:-1]) + "_edges_histogram.tsv"
print(f"Writing to {output_file}")
with open(output_file, 'w') as outputWriter:
    # Write header
    _ = list(map(str, histo_keys))
    _ = '\t'.join(_)
    outputWriter.write(f"compID\t{_}\n")
    for compID, edges in histogram_counts.items():
        pairs = []
        for pair in histo_keys:
            pairs.append(edges.get(pair, 0))

        pairs = list(map(str, pairs))
        _ = "\t".join(pairs)
        line = f"{compID}\t{_}\n"

        outputWriter.write(line)

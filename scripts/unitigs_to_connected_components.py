"""
This script is used under the project "Universal Transcriptome"

Input: Unitigs Fasta file generated from BCALM.
Output: CSV File with the following format

A. Column(1): Original Component ID
B. Column(2:): Unitigs ID(s)

Run:
python unitigs_to_connected_components.py <unitigs_path>
"""

# TODO: Print the size of component alongside its number of unitigs

import subprocess
from tqdm import tqdm
from collections import defaultdict
import sys
import os

if len(sys.argv) < 1:
    exit("ex: python unitigs_to_connected_components.py <unitigs_path>")

unitigs_path = sys.argv[1]

lines_number = int(subprocess.getoutput('wc -l ' + unitigs_path).split()[0]) // 2


def find(x):
    l = leaders[x]
    if l is not None:
        l = find(l)
        leaders[x] = l
        return l
    return x


def union(x, y):
    lx, ly = find(x), find(y)
    if lx != ly:
        leaders[lx] = ly


print("Parsing Fasta file ...")

source = []
target = []


def get_nodeID(link):
    return int(link[4:-2])


with open(unitigs_path, 'r') as unitigsReader:
    for line in tqdm(unitigsReader, total=lines_number):
        seq = next(unitigsReader).strip()
        header = line.strip().split()
        unitig_id = int(header[0][1:])
        links = list(map(get_nodeID, header[4:]))
        source.append(unitig_id)
        target.append(unitig_id)
        for link in links:
            source.append(unitig_id)
            target.append(link)

print("Generating connected components...")
leaders = defaultdict(lambda: None)

for i in range(len(source)):
    union(source[i], target[i])

groups = defaultdict(set)

for x in leaders:
    groups[find(x)].add(x)

print(f"number of components: {len(groups)}")

component_id = 1

output_file = os.path.basename(unitigs_path) + ".components.csv"
print(f"Dumping to {output_file}")

report = dict()

with open(output_file, 'w') as comp:
    for k, v in groups.items():
        report[k] = len(v)
        comp.write(f"{component_id}" + ',' + ",".join(list(map(str, v))) + '\n')
        component_id += 1

values = sorted(list(report.values()), reverse=True)

print(f"Top 30 components: {values[:30]}")

print("Done", file=sys.stderr)

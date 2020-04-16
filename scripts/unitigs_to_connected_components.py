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
from collections import Counter
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

unitig_to_size = dict()


def get_nodeID(link):
    return int(link[4:-2])


with open(unitigs_path, 'r') as unitigsReader:
    for line in tqdm(unitigsReader, total=lines_number):
        seq_len = len(next(unitigsReader).strip())
        header = line.strip().split()
        unitig_id = int(header[0][1:])
        unitig_to_size[unitig_id] = seq_len
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

output_file = os.path.basename(unitigs_path) + ".components.csv"
print(f"Dumping to {output_file}")

final_components = dict()
components_length = dict()

with open(output_file, 'w') as comp:
    for component_id, (k, v) in enumerate(groups.items(), start=1):
        # Save component with corrected component ID
        final_components[component_id] = v

        # Save component length
        components_length[component_id] = len(v)

        # Save component to CSV
        comp.write(f"{component_id}" + ',' + ",".join(list(map(str, v))) + '\n')

groups.clear()

top_30_ids = Counter(components_length).most_common(30)
components_size = list()
for comp in top_30_ids:
    compID = comp[0]
    compNodesSize = comp[1]
    _size_bp = 0

    """
    if not unitig_to_size.get(unitigID, 0):
        print(f"Couldn't find node {unitigID} in compID: {compID}")
    for i in range(len(source)):
        if source[i] == unitigID or target[i] == unitigID:
            print(f"at source, target: ({source[i]},{target[i]})")
    exit()
    """

    for unitigID in final_components[compID]:
        _size_bp += unitig_to_size.get(unitigID,0)
        # _size_bp += unitig_to_size[unitigID]

    components_size.append((compID, compNodesSize, _size_bp))

# Finding 3 highest values
high = k
print(f"Top 30 components: (ID, nodesCount, length_BP)\n{components_size}")

print("Done", file=sys.stderr)

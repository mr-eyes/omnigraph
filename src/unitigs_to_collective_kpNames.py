"""
This script is used under the project "Universal Transcriptome"

Input:
    1- Fasta file of the unitigs generated from BCALM
    2- collectiveComps_originalComps.csv (Written from the script originalComponentsToCollectiveComponents.py)

Output:
    1- Names file (TSV) ready to be used by the kProcessor

Run:
    - python unitigs_to_collective_kpNames.py <unitigs_fasta> <collectiveComps_csv>

Example:
    - python unitigs_to_collective_kpNames.py unitigs.fa collective.csv 

"""

import sys
import os
import subprocess
from tqdm import tqdm

if len(sys.argv) < 3:
    sys.exit("run: python unitigs_to_collective_kpNames.py <unitigs_fasta> <no_of_collective_components>")


unitigs_fasta = sys.argv[1]
collectiveCompCSV = sys.argv[2]

cmd = f"wc -l {collectiveCompCSV}"
originalCompsNo = int(subprocess.getoutput(cmd).split()[0])
print(f"Number of original components: {originalCompsNo}")

names_file = os.path.basename(unitigs_fasta) + ".names"


print("Mapping unitigs to collective components ...")
unitigToCollective = dict()
with open(collectiveCompCSV, 'r') as csvReader:
    for line in tqdm(csvReader, total = originalCompsNo):
        line = line.strip().split(',')
        collectiveCompID = line[1]
        for unitigID in line[2:]:
            unitigToCollective[unitigID] = collectiveCompID


cmd = f"wc -l {unitigs_fasta}"
noSeqs = int(subprocess.getoutput(cmd).split()[0])

print("Generating the names file ...")
with open(unitigs_fasta, 'r') as fastaReader, open(names_file, 'w') as namesWriter:
    for line in tqdm(fastaReader, total = noSeqs):
        if line[0] == '>':
            unitigID = line[1:].split()[0]
            collectiveCompID = unitigToCollective[unitigID]
            namesWriter.write(f"{line.strip()[1:]}\t{collectiveCompID}\n")
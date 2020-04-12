import os
import subprocess
from tqdm import tqdm
import sys
from collections import Counter

if len(sys.argv) < 2:
    sys.exit("run: python unitigs_edges_historgram.py <unitigs_fasta>")

unitigs_file = sys.argv[1]
no_seqs = int(subprocess.getoutput('wc -l ' + unitigs_file).split()[0]) // 2

histogram = list()

with open(unitigs_file, 'r') as unitigsReader:
    for line in tqdm(unitigsReader, total=no_seqs):
        next(unitigsReader)  # skip seq
        header = line.strip().split()
        all_links = header[4:]
        incoming = sum('L:-' in s for s in all_links) * -1
        outcoming = sum('L:+' in s for s in all_links)
        histogram.append((incoming, outcoming))

print(dict(Counter(histogram)))

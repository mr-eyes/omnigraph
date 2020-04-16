"""
This script is used under the project "Universal Transcriptome"

Input: Unitigs file generated from BCALM.
Output: new Unitigs fasta file with modified edges

Run:
python unitigs_dislinkage.py  <unitigs_fasta> <kSize> <mode:aggressive|default:mild>
"""

import os
import subprocess
from tqdm import tqdm
import sys
from statistics import median


class Dust:
    kSize = int()

    def __init__(self, kSize, mode="mild"):
        self.kSize = kSize

        if mode == "mild":
            self.initial = "ACTG"
        elif mode == "aggressive":
            self.initial = "ACTGGTCA"

        self.max_dust = self.maxDustWindow(read=str(self.initial * kSize)[0:kSize], window_size=kSize,
                                           min_window_size=kSize)
        print(f"Max dust: {self.max_dust}")
        print(f"Filteration mode: {mode}")
        # self.max_dust = self.medianDustWindow(read=str("ACTG" * kSize)[0:kSize], window_size=21, min_window_size=21)

    def maxDustWindow(self, read, window_size, min_window_size):
        max_score = 0.0
        r_l = len(read)
        for i in range(0, r_l - 1):
            r = r_l - i
            if r < window_size:
                w = r
            else:
                w = window_size
            if w >= min_window_size:
                s = self.calculateDustScore(read[i:i + w])
                if s > max_score:
                    max_score = s

        return max_score

    def medianDustWindow(self, read, window_size=21, min_window_size=21):
        max_scores = []
        r_l = len(read)
        for i in range(0, r_l - 1):
            r = r_l - i
            if r < window_size:
                w = r
            else:
                w = window_size
            if w >= min_window_size:
                s = self.calculateDustScore(read[i:i + w])
                max_scores.append(s)

        return median(max_scores)

    def calculateDustScore(self, read):
        dec = {}
        r_l = len(read)
        if r_l < 3:
            return 0.0
        for i in range(0, r_l - 2):
            s = read[i:i + 3]
            if s not in dec:
                dec[s] = 1
            else:
                dec[s] = dec[s] + 1
        sum_val = 0.0
        for i in range(0, len(dec)):
            tc = float(list(dec.values())[i])
            score = (tc * (tc - 1)) / 2.0
            sum_val = sum_val + score

        return sum_val / (r_l - 2)


if len(sys.argv) < 3:
    sys.exit("run: python unitigs_dislinkage.py <unitigs_fasta> <kSize> <mode:aggressive|default:mild>")

filter_mode = "mild"

if len(sys.argv) == 4:
    mode = sys.argv[3]

unitigs_file = sys.argv[1]

LENGTH_THRESHOLD = 151

if not os.path.exists(unitigs_file):
    sys.exit(f"File {unitigs_file} does not exist.")

# make it k-1
kSize = int(sys.argv[2]) - 1

# unitigs_file = "test.unitigs.fa"
# unitigs_file = "/home/mabuelanin/Desktop/dev-plan/omnigraph/data/cDBG_SRR11015356_k31.unitigs.fa"

no_seqs = int(subprocess.getoutput('wc -l ' + unitigs_file).split()[0]) // 2

DUST = Dust(kSize, mode=filter_mode)

output_file = str()
if filter_mode == "mild":
    output_file = "mild_dislinked_" + os.path.basename(unitigs_file)
else:
    output_file = "aggressive_dislinked_" + os.path.basename(unitigs_file)

print(f"Parsing {unitigs_file} ...")

with open(unitigs_file, 'r') as unitigsReader, open(output_file, 'w') as unitigsWriter:
    for line in tqdm(unitigsReader, total=no_seqs):
        line = line.strip()
        seq = next(unitigsReader).strip()
        header = line.strip().split()

        new_header = " ".join(header[:4]) + "  "
        all_links = header[4:]

        should_be_removed = list()

        first_k = bool(DUST.calculateDustScore(seq[:DUST.kSize]) > DUST.max_dust)
        last_k = bool(DUST.calculateDustScore(seq[-DUST.kSize:]) > DUST.max_dust)

        if first_k and last_k:
            if len(seq) <= LENGTH_THRESHOLD:
                continue

        if first_k:
            should_be_removed.append("L:-")
        if last_k:
            should_be_removed.append("L:+")

        for link in all_links:
            if link[:3] not in should_be_removed:
                new_header += " " + link

        unitigsWriter.write(f"{new_header}\n{seq}\n")

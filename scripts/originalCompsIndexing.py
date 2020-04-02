"""
This script is used under the project "Universal Transcriptome"

Input 1: Unitigs fasta file:
    - each file should be in the following format {collectiveCompID}_*_fa.names

Input 2: Names Files Directory:
    - each file should be in the following format {collectiveCompID}_*_fa.names

Input 3: Number of cores, default(1)


Output: Directory of directories:
    - each subdirectory will contains kProcessor index

Run:
    python originalCompsIndexing.py <unitigs_fasta> <namesFiles_Dir> <no_cores:1>

Example:
    python originalCompsIndexing.py unitgs.fa originalComponents 8

"""

import sys
import subprocess
from tqdm import tqdm
import os
import shutil
import glob
import kProcessor as kp
import time
import multiprocessing

names = list()
fasta_file = str()
no_cores = 1
namesFileDir = str()

if len(sys.argv) < 3:
    sys.exit("run: python originalCompsIndexing.py <unitigs_fasta> <namesFiles_Dir> <no_cores>")
else:
    fasta_file = sys.argv[1]
    namesFileDir = sys.argv[2]
    if len(sys.argv) == 4:
        no_cores = int(sys.argv[3])


def index(names_file):
    global fasta_file
    idx_suffix = os.path.basename(names_file).replace(".fa.names", "")
    print(f"Indexing {idx_suffix} ...", file = sys.stderr)
    kSize = 31
    kmers_mode = 1
    hashing_mode = 1
    chunk_size = 100000
    kf_PHMAP = kp.kDataFramePHMAP(kSize, hashing_mode)
    ckf = kp.index(kf_PHMAP, {"mode" : kmers_mode}, fasta_file , chunk_size, names_file)
    os.mkdir(f"idx_{idx_suffix}")
    ckf.save(f"idx_{idx_suffix}/idx_{idx_suffix}")


for namesFile in glob.glob(f"{namesFileDir}/*fa.names"):
    names.append(namesFile)

with multiprocessing.Pool(no_cores) as pool:
    pool.map(index, names)

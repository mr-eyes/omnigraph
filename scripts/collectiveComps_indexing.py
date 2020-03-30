"""
This script is used under the project "Universal Transcriptome"

Input:
    - Fasta file

Output:
    - Directory contains kProcessor index

Run:
    python collectiveComps_indexing.py <fasta_file>

Example:
    python collectiveComps_indexing.py transcripts.fa

"""

import kProcessor as kp
import os
import sys
from configparser import ConfigParser
import time
from datetime import timedelta

def read_kp_config(filename='config.ini', section='kProcessor'):
    parser = ConfigParser()
    parser.read(filename)
    params = {}
    
    if parser.has_section(section):
        items = parser.items(section)
        for item in items:
            params[item[0]] = int(item[1])
    else:
        raise Exception('{0} not found in the {1} file'.format(section, filename))
 
    return params


if len(sys.argv) < 2:
    sys.exit("run: python collectiveComps_indexing.py <fasta_file>")

fasta_file = sys.argv[1]
names_file=fasta_file + ".names"

idx_dir = "idx_" + ''.join(os.path.basename(fasta_file).split('.')[:-1])

params = read_kp_config()

if not os.path.exists(idx_dir):
    os.makedirs(idx_dir)

kSize = params["ksize"]
kmers_mode = params["kmers_mode"] # Kmers
hashing_mode = params["hashing_mode"] # Integer
chunk_size = params["chunk_size"]
Q = params["q"]

kf_MQF = kp.kDataFrameMQF(kSize, Q , hashing_mode)
# kf_MQF = kp.kDataFramePHMAP(kSize, hashing_mode)

start_time = time.time()

print("Indexing ...")
ckf = kp.index(kf_MQF, {"mode" : kmers_mode}, fasta_file , chunk_size, names_file)

indexing_time = time.time() - start_time

print("saving cKF ...")

ckf.save(idx_dir + '/' + "mqf_idx_" + idx_dir)

kf = ckf.getkDataFrame()

print(f"""
Indexing time : {timedelta(seconds=indexing_time)}
Saving time: {timedelta(seconds=(time.time() - start_time))}
Number of kmers: {kf.size()}
""")
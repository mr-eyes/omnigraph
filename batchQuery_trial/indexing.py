import kProcessor as kp
import sys

fasta_file = sys.argv[1]
names_file = sys.argv[2]

kf = kp.kDataFrameMQF(21, 29, 1)

# Mode 1 activates the kmers mode
ckf = kp.index(kf, {"mode" : 1}, fasta_file , 10000, names_file)

print("Serializing the index ...")

ckf.save("index/idx_gencode.v32.transcripts")
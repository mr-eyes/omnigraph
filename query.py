import kProcessor as kp


idx = kp.colored_kDataFrame.load("index/idx_gencode.v32.transcripts")

print(idx.getColor("TTCATCTTCTGGCCCCTGTTGTCTG"))

batchQuery = kp.ckf_batchQuery(idx, "read.fa", {"mode" : 1, "k_size" : 25} , 1)

batchQuery.next()

print(batchQuery.get_transcripts())
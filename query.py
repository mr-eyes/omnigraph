import kProcessor as kp


idx = kp.colored_kDataFrame.load("index/idx_gencode.v32.transcripts")



batchQuery = kp.ckf_batchQuery(idx, "read.fa", {"mode" : 1, "k_size" : 21} , 1)

while not batchQuery.end():
    batchQuery.next()
    print(batchQuery.get_transcripts())
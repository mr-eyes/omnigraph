import kProcessor as kp

idx = kp.colored_kDataFrame.load("index/idx_gencode.v32.transcripts")

names_map = idx.names_map()

batchQuery = kp.ckf_batchQuery(idx, "read.fa", {"mode" : 1, "k_size" : 21} , 1)

all_results = dict()

while not batchQuery.end():
    batchQuery.next()
    result = batchQuery.get_transcripts()
    
    for read_id, kmers in result.items():
        score = dict()
        flattened = []
        
        for kmer in kmers:
            flattened += [x for x in kmer]
        
        most_frequent = max(set(flattened), key = flattened.count)
        all_results[read_id] = most_frequent


for read_id, matched_transcript in all_results.items():
    print(f"Query Read: {read_id}")
    print(f"Matched Transcript: {names_map[matched_transcript]}")
    print("---------------------")
    
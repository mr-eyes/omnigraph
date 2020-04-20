# Six GFAs analysis

## GFAs

1. original cDBG75
2. mild dislinked cDBG75
3. aggressive dislinked cDBG75
4. reduced original cDBG75 @ 98.5
5. reduced mild dislinked cDBG75 cDBG75 @ 98.5
6. reduced aggressive dislinked cDBG75 cDBG75 @ 98.5

## Seqkit stats

```tsv
file                                                  format  type    num_seqs        sum_len  min_len  avg_len  max_len
cDBG_SRR11015356_k75.unitigs.fa                       FASTA   DNA   11,824,622  1,133,741,131       75     95.9    1,683
mild_dislinked_cDBG_SRR11015356_k75.unitigs.fa        FASTA   DNA   11,766,538  1,129,128,323       75       96    1,683
aggressive_dislinked_cDBG_SRR11015356_k75.unitigs.fa  FASTA   DNA   11,672,056  1,121,294,392       75     96.1    1,683
original_reduced_985_cDBG75.fa                        FASTA   DNA    4,881,230    572,060,545       75    117.2    4,574
reduced_985_mild_dislinked_cDBG75.fa                  FASTA   DNA    4,868,502    570,966,694       75    117.3    4,574
reduced_985_aggressive_dislinked_cDBG75.fa            FASTA   DNA    4,839,097    568,239,807       75    117.4    4,574
```

## Labeling

```bash

original=cDBG_SRR11015356_k75
mild=mild_dislinked_cDBG_SRR11015356_k75
aggressive=aggressive_dislinked_cDBG_SRR11015356_k75
reduced_original=original_reduced_985_cDBG75
reduced_mild=reduced_985_mild_dislinked_cDBG75
reduced_aggressive=reduced_985_aggressive_dislinked_cDBG75

for unitigs_fasta in ${original} ${mild} ${aggressive} ${reduced_original} ${reduced_mild} ${reduced_aggressive};
do
    echo -e "labeling $unitigs_fasta ... \n";
    ./cDBG_labeling ${unitigs_fasta}.unitigs.fa ${unitigs_fasta}.unitigs.fa.names.tsv ${unitigs_fasta}
done

```

## **Summary stats**


### 1. Original

```text
Summary report: 

Paired End File: 1 | mapped_reads  %93.02
Scenario (1) : Count: 58833610 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 435623 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 4173579 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 134035 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4377516 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
---------------------------------
Paired End File: 2 | mapped_reads  %88.709
Scenario (1) : Count: 54225132 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 482002 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 6983279 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 207414 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 6056536 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
```

---

### 2. Aggressive Dislinked

```text
Summary report: 

Paired End File: 1 | mapped_reads  %93.02
Scenario (1) : Count: 58833610 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 435623 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 4173579 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 134035 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4377516 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
---------------------------------
Paired End File: 2 | mapped_reads  %88.709
Scenario (1) : Count: 54225132 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 482002 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 6983279 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 207414 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 6056536 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
```

---

### 3. Mild Dislinked

```text
Summary report: 

Paired End File: 1 | mapped_reads  %93.1076
Scenario (1) : Count: 58955315 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 438400 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 4109364 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 135897 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4315387 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
---------------------------------
Paired End File: 2 | mapped_reads  %88.7899
Scenario (1) : Count: 54325783 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 484313 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 6923940 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 209479 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 6010848 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
```

---

### 4. Reduced original @ 98.5
```text
Summary report: 

Paired End File: 1 | mapped_reads  %14.6339
Scenario (1) : Count: 5465615 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 2541167 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 54074538 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 1394257 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4478786 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
---------------------------------
Paired End File: 2 | mapped_reads  %13.9033
Scenario (1) : Count: 5006152 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 2374020 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 54751787 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 1380689 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4441715 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
```


### 5. Reduced Mild Dislinked @ 98.5
```text
Summary report: 

Paired End File: 1 | mapped_reads  %14.6338
Scenario (1) : Count: 5465221 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 2540357 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 54076609 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 1393076 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4479100 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
---------------------------------
Paired End File: 2 | mapped_reads  %13.8917
Scenario (1) : Count: 5000571 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 2373347 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 54761715 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 1379274 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4439456 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
```

### 6. Reduced Aggressive Dislinked @ 98.5
```text
Summary report: 

Paired End File: 1 | mapped_reads  %14.6266
Scenario (1) : Count: 5458119 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 2534407 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 54092772 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 1387755 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4481310 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
---------------------------------
Paired End File: 2 | mapped_reads  %13.8862
Scenario (1) : Count: 4995366 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 2369132 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 54774473 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 1374475 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 4440917 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
```

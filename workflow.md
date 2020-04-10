# Workflow

## 1. Creating the cDBG

**Steps:**

### 1.1 BCalm cDBG

```bash
cd data/

ls -1 *fasta > list_reads
touch 1.1_cDBG_log.txt
/usr/bin/time -v bcalm -kmer-size 31 -abundance-min 3 -max-memory 12000 -out cDBG_SRR11015356_k31 -in list_reads &> 1.1_cDBG_log.txt

# Convert to GFA

wget https://raw.githubusercontent.com/GATB/bcalm/master/scripts/convertToGFA.py
python convertToGFA.py cDBG_SRR11015356_k31.unitigs.fa cDBG_SRR11015356_k31.unitigs.gfa 31 --single-directed

```

**cDBG Stats**

```txt
file                             format  type   num_seqs      sum_len  min_len  avg_len  max_len
cDBG_SRR11015356_k31.unitigs.fa  FASTA   DNA   9,145,177  447,026,231       31     48.9    5,027
```

---

## 2 Connected components

### 2.1 Initialize mySQL

1. Install mySQL
2. From the PhpMyAdmin or from the SQL command CLI, create the DB:
    - Login as root
    - `CREATE DATABASE universal_transcriptome;`
3. Create the unitigs table:
    - [mySQL Commands](./src/mysql_scripts/create_unitigs_tracking.sql)

#### 2.2 Get the connected components from the GFA

```bash
python scripts/gfa_to_connected_components.py data/cDBG_SRR11015356_k31.unitigs.gfa

<<STATS

Top 30 components: [7725860, 2559, 1966, 1832, 1603, 1523, 1522, 1489, 1362, 1316, 1278, 1268, 1247, 1229, 1194, 1180, 1173, 1171, 1129, 1064, 1046, 999, 996, 995, 961, 924, 922, 909, 859, 856]

(Mystats)
Number of original connected components : 1,032,950

(BandageStats)
Node count:                       9,145,177
Edge count:                       11,701,516
Total length (bp):                447026231
Connected components:             1,032,950 SAME AS MINE
Largest component (bp):           343,561,868
Shortest node (bp):               31

STATS

```

#### 2.3 Trial to performe edges dislinkage

```bash
python kExplorer/graph_analysis/1-dislinkage.py cDBG_SRR11015356_k31.unitigs.gfa 31
python scripts/gfa_to_connected_components.py dislinked_cDBG_SRR11015356_k31.unitigs.gfa

<<STATS

Top 30 components: [7432320, 2739, 2553, 1966, 1877, 1832, 1807, 1660, 1603, 1554, 1527, 1523, 1522, 1516, 1489, 1454, 1362, 1316, 1278, 1268, 1247, 1245, 1229, 1204, 1194, 1190, 1180, 1174, 1173, 1171]

(Mystats)
Number of original connected components : 1,201,346

(BandageStats)
Node count:                       9,145,177
Edge count:                       11,411,972
Total length (bp):                447026231
Connected components:             1,201,346
Largest component (bp):           329,952,460 (diff: 13,609,408 bp)
Shortest node (bp):               31

STATS

```


#### 2.3 Add the collective components column (50k) collective component

```bash
python scripts/originalComponentsToCollectiveComponents.py cDBG_SRR11015356_k31.unitigs.gfa.components.csv 180
```

#### 2.4 Insert the original & collective components into the mySQL unitigs_tracking table

```bash
python mySQL_insert_components.py collectiveComps_cDBG_SRR11015356_k31.unitigs.gfa.components.tsv
```

> time: ~17 mins
> Size on disk: 885.6 MiB due to the multiple indeces.

---

### 3 kProcessor ***Hierarchical*** Indexing

#### 3.1 Generate names file for kProcessor <unitig_header:collectiveCompID>

```bash
python scripts/unitigs_to_collective_kpNames.py data/cDBG_SRR11015356_k31.unitigs.fa collectiveComps_cDBG_SRR11015356_k31.unitigs.gfa.components.csv
mv cDBG_SRR11015356_k31.unitigs.fa.names data/
```

#### 3.2 Calculate the number of unique canonical kmers in the unitigs

```bash
UNITIGS=data/cDBG_SRR11015356_k31.unitigs.fa
jellyfish count -m 31 -s 250M -t 10 -C ${UNITIGS}
jellyfish dump mer_counts.jf > ${UNITIGS}.kmers.fa
cat ${UNITIGS}.kmers.fa | grep ">" | wc -l
```

> Number of kmers **172,670,656**

#### 3.3 **(PHMAP)** Indexing the 180 Collective component (First level indexing)

> [TODO] make the script takes the names file separately

```bash
python scripts/collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa
```

- **Time:** 00:02:30
- **Mem:** 8.9 GB

#### 3.4 Perform first query

This step creates a sqlite DB with columns as following

- Unique serial ID
- PE Read 1 (original or trimmed)
- PE Read 2 (original or trimmed)
- R1 Collective component ID
- R2 Collective component ID

```bash
./build/query_1 # Optional: # --db /query_1.db

<<RESULT
Paired End File: 1 | mapped_reads  %97.3955
Scenario (1) : Count: 61043395 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 283071 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 1279901 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 206889 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 5141107 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
---------------------------------
Paired End File: 2 | mapped_reads  %94.5279
Scenario (1) : Count: 57128736 | Mapped: from matching the first and last kmers only.
Scenario (2) : Count: 323910 | Unmapped: Both terminal kmers matched but on different components.
Scenario (3) : Count: 3030808 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers unmatched.
Scenario (4) : Count: 363791 | Unmapped: One or both of the terminal kmers not matched & > %50 of kmers matched with colors intersecton > 1.
Scenario (5) : Count: 7107118 | Mapped: Partial match and read is trimmed.
Scenario (6) : Count: 0 | Unmapped: There's no single matched kmer.
RESULT

```

- **Time:** 3:49:20
- **Mem:** 6.56 GB
- **DB FIle Size:** 24 GB


##### Sqlite DB insights **[Debugging purposes]**

```sqlite
select seq1_collective_component, count(seq1_collective_component) from reads group by seq1_collective_component;
select seq2_collective_component, count(seq2_collective_component) from reads group by seq2_collective_component;
```


#### 3.4 Perform second query

```bash
./query_2 # Optional: # --db query_1.db --out-dir fasta_out
```

- **Time:** 1:50:07
- **Mem:** 5.01 GB

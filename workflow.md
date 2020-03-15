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
python src/gfa_to_connected_components.py data/cDBG_SRR11015356_k31.unitigs.gfa
```

#### 2.3 Add the collective components column (50k) collective component

```bash
python src/originalComponentsToCollectiveComponents.py cDBG_SRR11015356_k31.unitigs.gfa.components.csv 50000
```

#### 2.4 Insert the original & collective components into the mySQL unitigs_tracking table

```bash
python mySQL_insert_components.py collectiveComps_cDBG_SRR11015356_k31.unitigs.gfa.components.tsv
```

> time: ~17 mins
> Size on disk: 885.6 MiB due to the multiple indeces.

---

### 3 kProcessor Indexing

#### 3.1 Generate names file for kProcessor <unitig_header:collectiveCompID>

```bash
python unitigs_to_collective_kpNames.py data/cDBG_SRR11015356_k31.unitigs.fa collectiveComps_cDBG_SRR11015356_k31.unitigs.gfa.components.tsv
```

#### 3.2 Calculate the number of unique canonical kmers in the unitigs

```bash
UNITIGS=data/cDBG_SRR11015356_k31.unitigs.fa
jellyfish count -m 31 -s 250M -t 10 -C ${UNITIGS}
jellyfish dump mer_counts.jf > ${UNITIGS}.kmers.fa
cat ${UNITIGS}.kmers.fa | grep ">" | wc -l
```

> Number of kmers **172,670,656**

#### 3.3 **(MQF)** Indexing the 50K Collective component

##### Trial 1 (-56,404,746 Kmers)

```ini
[kProcessor]
ksize = 31
Q = 27
hashing_mode = 1
kmers_mode = 1
chunk_size = 100000
```

```bash
python collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa
```

Stats

```txt
Indexing time : 0:07:49.865936
Number of kmers: 116265910
Index size: 1.4 GB
```

##### Trial 2 (-47,567,281 Kmers)

```ini
[kProcessor]
ksize = 31
Q = 28
hashing_mode = 1
kmers_mode = 1
chunk_size = 100000
```

```bash
python collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa
```

Stats

```txt
Indexing time : 0:05:54.605237
Memory: 3.7 GB
Number of kmers: 125103375
Index size: 1.4 GB
```

##### Trial 3 (-23,783,608 Kmers)

```ini
[kProcessor]
ksize = 31
Q = 29
hashing_mode = 1
kmers_mode = 1
chunk_size = 100000
```

```bash
python collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa
```

Stats

```txt
Indexing time : 0:05:02.889045
Memory: 5GB
Number of kmers: 148887048
Index size: 2.8 GB
```

##### Trial 4 (-11,891,562 Kmers)

```ini
[kProcessor]
ksize = 31
Q = 30
hashing_mode = 1
kmers_mode = 1
chunk_size = 100000
```

```bash
python collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa
```

Stats

```txt
Indexing time : 0:04:06.227647
Memory: 7.5 GB
Number of kmers: 160779094
Index size: 5.4 GB
```

##### Trial 5 (-5,946,610 Kmers) **The maximum I can afford**

```ini
[kProcessor]
ksize = 31
Q = 31
hashing_mode = 1
kmers_mode = 1
chunk_size = 100000
```

```bash
python collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa
```

Stats

```txt
Indexing time : 0:04:03.444587
Memory: 12.5 GB
Number of kmers: 166724046
Index size: 10.5 GB
```

### **PHMAP Surprise**

Inline-style:
<img src="https://www.pngfind.com/pngs/m/9-94551_graphic-ballon-vector-surprise-box-sorpresa-comic-png.png" alt="drawing" width="200"/>

#### Trial 1 (No Lost Kmers!!!)

```ini
[kProcessor]
ksize = 31
hashing_mode = 1
kmers_mode = 1
chunk_size = 100000
```

```bash
python collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa
```

Stats

```txt
Indexing time : 0:02:49.239873
Memory: 9 GB
Number of kmers: 172670656
Index size: 2.8 GB
```

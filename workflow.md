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

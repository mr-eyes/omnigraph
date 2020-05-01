# Workflow k75

## Description

The following steps will guide through running the whole pipeline at KMER_SIZE = 75.

Key points:

1. Main Data preparation
    - 1.1 Downloading Human SRA rna-seq PE reads.
    - 1.2 Creating compact De Bruijn graph for the reads & getting stats.
2. cDBG Transformation
    - 2.1 Graph dislinking and filtration.
        - 2.1.1 Mild dislinkage
        - 2.1.2 Aggressive dislinkage
    - 2.2 CD-HIT Clustering @ sim [0.95, 0.97, 0.985]
        - 2.2.1 Mild dislinkage clustering
        - 2.2.2 Aggressive dislinkage clustering

3. Partitioning
    - Indexing
    - Raw reads matching
    - Dumping

---

## 1. Main Data preparation

### 1.1 Downloading Human SRA rna-seq PE reads.
```bash
wget -c https://sra-download.ncbi.nlm.nih.gov/traces/sra51/SRR/010757/SRR11015356 -O SRR11015356.sra
fastq-dump --fasta 0 --split-files SRR11015356.sra
```

### 1.2 Creating compact De Bruijn graph for the reads.
```shell script

# Creating a file with reads paths
ls -1 *fasta > list_reads

KMER_SIZE=75
MAX_MEMORY=12000
MIN_ABUNDANCE=1
OUTPUT_PREFIX=cDBG_SRR11015356_k75

STD_OUT=stats_original_cDBG${KMER_SIZE}.txt

SCRIPTS=scripts/ # scripts path in omnigraph/scripts

# Constructing cDBG
bcalm -kmer-size ${KMER_SIZE} -abundance-min ${MIN_ABUNDANCE} -max-memory ${MAX_MEMORY} -out ${OUTPUT_PREFIX} -in list_reads

# Convert to GFA (Just for bandage)
python ${SCRIPTS}/unitigsToGFA.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.gfa ${KMER_SIZE} --single-directed


## Getting some stats
echo -e "Bandage info:\n" > ${STD_OUT}
Bandage info ${OUTPUT_PREFIX} >>${STD_OUT} 2>&1

## Connected components
echo -e "Unitigs to connected components:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_connected_components.py ${OUTPUT_PREFIX}.unitigs.fa >> ${STD_OUT}
largest_component=$(cat ${STD_OUT}| grep -w "largest_comp_bp" | cut -f2)

## Getting LN&KM Stats
echo -e "LM & KM Stats:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv ${largest_component} >>  ${STD_OUT}

## Edges Histogram
python ${SCRIPTS}/unitigs_edges_histogram.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv

```

---

## 2. cDBG Transformation

### 2.1 Graph dislinking and filtration.

#### 2.1.1 Mild dislinkage

```shell script

KMER_SIZE=75
FILTER_MODE=mild # << Discriminator variable
SCRIPTS=scripts/ # scripts path in omnigraph/scripts
ORIGINAL_cDBG_PREFIX=cDBG_SRR11015356_k75
STD_OUT=stats_${FILTER_MODE}_${ORIGINAL_cDBG_PREFIX}.txt

## Apply mild dislinkage
OUTPUT_PREFIX=${FILTER_MODE}_dislinked_${ORIGINAL_cDBG_PREFIX}
python ${SCRIPTS}/unitigs_dislinkage.py ${ORIGINAL_cDBG_PREFIX}.unitigs.fa  ${KMER_SIZE} ${FILTER_MODE}

# Convert to GFA (Just for bandage)
python ${SCRIPTS}/unitigsToGFA.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.gfa ${KMER_SIZE} --single-directed


## Getting some stats
echo -e "Bandage info:\n" > ${STD_OUT}
Bandage info ${OUTPUT_PREFIX} >>${STD_OUT} 2>&1

## Connected components
echo -e "Unitigs to connected components:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_connected_components.py ${OUTPUT_PREFIX}.unitigs.fa >> ${STD_OUT}
largest_component=$(cat ${STD_OUT}| grep -w "largest_comp_bp" | cut -f2)

## Getting LN&KM Stats
echo -e "LM & KM Stats:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv ${largest_component} >>  ${STD_OUT}

## Edges Histogram
python ${SCRIPTS}/unitigs_edges_histogram.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv

```


#### 2.1.2 Aggressive dislinkage

```shell script

KMER_SIZE=75
FILTER_MODE=aggressive # << Discriminator variable
SCRIPTS=scripts/ # scripts path in omnigraph/scripts
ORIGINAL_cDBG_PREFIX=cDBG_SRR11015356_k75
STD_OUT=stats_${FILTER_MODE}_${ORIGINAL_cDBG_PREFIX}.txt
OUTPUT_PREFIX=${FILTER_MODE}_dislinked_${ORIGINAL_cDBG_PREFIX}
## Apply mild dislinkage

python ${SCRIPTS}/unitigs_dislinkage.py ${ORIGINAL_cDBG_PREFIX}.unitigs.fa  ${KMER_SIZE} ${FILTER_MODE}

# Convert to GFA (Just for bandage)
python ${SCRIPTS}/unitigsToGFA.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.gfa ${KMER_SIZE} --single-directed


## Getting some stats
echo -e "Bandage info:\n" > ${STD_OUT}
Bandage info ${OUTPUT_PREFIX} >>${STD_OUT} 2>&1

## Connected components
echo -e "Unitigs to connected components:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_connected_components.py ${OUTPUT_PREFIX}.unitigs.fa >> ${STD_OUT}
largest_component=$(cat ${STD_OUT}| grep -w "largest_comp_bp" | cut -f2)

## Getting LN&KM Stats
echo -e "LM & KM Stats:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv ${largest_component} >>  ${STD_OUT}

## Edges Histogram
python ${SCRIPTS}/unitigs_edges_histogram.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv

```

### 2.2 CD-HIT Clustering @ sim [0.95, 0.97, 0.985]

#### 2.2.1 Mild dislinkage clustering

```shell script

KMER_SIZE=75
FILTER_MODE=mild # << Discriminator variable
SCRIPTS=scripts/ # scripts path in omnigraph/scripts
ORIGINAL_cDBG_PREFIX=cDBG_SRR11015356_k75
STD_OUT=stats_reduced_${FILTER_MODE}_${ORIGINAL_cDBG_PREFIX}.txt
OUTPUT_PREFIX=${FILTER_MODE}_dislinked_${ORIGINAL_cDBG_PREFIX}

THREADS=32
OUT_PREFIX=cDBG75
MAX_RAM_MB=64000
MAX_RAM_GB=64

echo "CD-HIT on %95 ..."
WORD_SIZE=9
SIM=0.95
cd-hit-est -i cDBG_${OUTPUT_PREFIX}.unitigs.fa -n ${WORD_SIZE} -c ${SIM} -o clusters_${SIM}_${OUT_PREFIX} -d 0 -T ${THREADS} -M ${MAX_RAM_MB}

WORD_SIZE=11
echo "CD-HIT on %97 ..."
SIM=0.97
cd-hit-est -i cDBG_${OUTPUT_PREFIX}.unitigs.fa -n ${WORD_SIZE} -c ${SIM} -o clusters_${SIM}_${OUT_PREFIX} -d 0 -T ${THREADS} -M ${MAX_RAM_MB}

echo "CD-HIT on %98.5 ..."
SIM=0.985
cd-hit-est -i cDBG_${OUTPUT_PREFIX}.unitigs.fa -n ${WORD_SIZE} -c ${SIM} -o clusters_${SIM}_${OUT_PREFIX} -d 0 -T ${THREADS} -M ${MAX_RAM_MB}

## EXTRACTING Representatives from REF_FASTA

for SIM in 0.95 0.97 0.985
do
    echo "Extracting representatives from clusters at %${SIM}"
    cat clusters_${SIM}_${OUT_PREFIX}.clstr | grep "\*" | awk -F"[>.]" '{print ">"$2}' | grep -Fwf - -A1 <(seqkit seq -w 0 ${ORIGINAL_cDBG_PREFIX}.unitigs.fa) | grep -v "^\-\-" > reps_unitigs_${OUT_PREFIX}_${SIM}.fa
done

## Stats on representatives

for SIM in 0.95 0.97 0.985
do
    REPS_STD_OUT=stats_reps_unitigs_${OUT_PREFIX}_${SIM}.txt
    python ${SCRIPTS}/unitigs_to_connected_components.py reps_unitigs_${OUT_PREFIX}_${SIM}.fa > ${REPS_STD_OUT}
    largest_component=$(cat ${REPS_STD_OUT}| grep -w "largest_comp_bp" | cut -f2)
    python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py reps_unitigs_${OUT_PREFIX}_${SIM}.fa reps_unitigs_${OUT_PREFIX}_${SIM}.fa.components.csv ${largest_component} >>  "${SIM}_${STD_OUT}"
done


## Constructing cDBG of the representative sequences

for SIM in 0.95 0.97 0.985
do
    echo "Constructing cDBG of representatives at %${SIM}"
    bcalm -kmer-size ${KMER_SIZE} -nb-cores ${THREADS} -max-memory ${MAX_RAM_MB} -abundance-min 1 -out cDBG2_${SIM}_${OUT_PREFIX} -in reps_unitigs_${OUT_PREFIX}_${SIM}.fa
    echo "Converting reps_unitigs_${OUT_PREFIX}_${SIM}.fa to GFA"
    python ${SCRIPTS}/unitigsToGFA.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.gfa ${KMER_SIZE} --single-directed
    echo "Getting cDBG Stats for cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa"
    Bandage info cDBG2_${SIM}_${OUT_PREFIX}.unitigs.gfa &> "${SIM}_${STD_OUT}"
    python ${SCRIPTS}/unitigs_to_connected_components.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa >> "${SIM}_${STD_OUT}"
    python ${SCRIPTS}/unitigs_edges_histogram.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa.components.csv
    largest_component=$(cat ${STD_OUT}| grep -w "largest_comp_bp" | cut -f2)
    python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa.components.csv ${largest_component} >>  "${SIM}_${STD_OUT}"    
done

```

#### 2.2.2 Aggressive dislinkage clustering

```shell script

KMER_SIZE=75
FILTER_MODE=aggressive # << Discriminator variable
SCRIPTS=scripts/ # scripts path in omnigraph/scripts
ORIGINAL_cDBG_PREFIX=cDBG_SRR11015356_k75
STD_OUT=stats_reduced_${FILTER_MODE}_${ORIGINAL_cDBG_PREFIX}.txt
OUTPUT_PREFIX=${FILTER_MODE}_dislinked_${ORIGINAL_cDBG_PREFIX}

THREADS=32
OUT_PREFIX=cDBG75
MAX_RAM_MB=64000
MAX_RAM_GB=64

echo "CD-HIT on %95 ..."
WORD_SIZE=9
SIM=0.95
cd-hit-est -i cDBG_${OUTPUT_PREFIX}.unitigs.fa -n ${WORD_SIZE} -c ${SIM} -o clusters_${SIM}_${OUT_PREFIX} -d 0 -T ${THREADS} -M ${MAX_RAM_MB}

WORD_SIZE=11
echo "CD-HIT on %97 ..."
SIM=0.97
cd-hit-est -i cDBG_${OUTPUT_PREFIX}.unitigs.fa -n ${WORD_SIZE} -c ${SIM} -o clusters_${SIM}_${OUT_PREFIX} -d 0 -T ${THREADS} -M ${MAX_RAM_MB}

echo "CD-HIT on %98.5 ..."
SIM=0.985
cd-hit-est -i cDBG_${OUTPUT_PREFIX}.unitigs.fa -n ${WORD_SIZE} -c ${SIM} -o clusters_${SIM}_${OUT_PREFIX} -d 0 -T ${THREADS} -M ${MAX_RAM_MB}

## EXTRACTING Representatives from REF_FASTA

for SIM in 0.95 0.97 0.985
do
    echo "Extracting representatives from clusters at %${SIM}"
    cat clusters_${SIM}_${OUT_PREFIX}.clstr | grep "\*" | awk -F"[>.]" '{print ">"$2}' | grep -Fwf - -A1 <(seqkit seq -w 0 ${ORIGINAL_cDBG_PREFIX}.unitigs.fa) | grep -v "^\-\-" > reps_unitigs_${OUT_PREFIX}_${SIM}.fa
done

## Stats on representatives

for SIM in 0.95 0.97 0.985
do
    REPS_STD_OUT=stats_reps_unitigs_${OUT_PREFIX}_${SIM}.txt
    python ${SCRIPTS}/unitigs_to_connected_components.py reps_unitigs_${OUT_PREFIX}_${SIM}.fa > ${REPS_STD_OUT}
    largest_component=$(cat ${REPS_STD_OUT}| grep -w "largest_comp_bp" | cut -f2)
    python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py reps_unitigs_${OUT_PREFIX}_${SIM}.fa reps_unitigs_${OUT_PREFIX}_${SIM}.fa.components.csv ${largest_component} >>  "${SIM}_${STD_OUT}"
done


## Constructing cDBG of the representative sequences

for SIM in 0.95 0.97 0.985
do
    echo "Constructing cDBG of representatives at %${SIM}"
    bcalm -kmer-size ${KMER_SIZE} -nb-cores ${THREADS} -max-memory ${MAX_RAM_MB} -abundance-min 1 -out cDBG2_${SIM}_${OUT_PREFIX} -in reps_unitigs_${OUT_PREFIX}_${SIM}.fa
    echo "Converting reps_unitigs_${OUT_PREFIX}_${SIM}.fa to GFA"
    python ${SCRIPTS}/unitigsToGFA.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.gfa ${KMER_SIZE} --single-directed
    echo "Getting cDBG Stats for cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa"
    Bandage info cDBG2_${SIM}_${OUT_PREFIX}.unitigs.gfa &> "${SIM}_${STD_OUT}"
    python ${SCRIPTS}/unitigs_to_connected_components.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa >> "${SIM}_${STD_OUT}"
    python ${SCRIPTS}/unitigs_edges_histogram.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa.components.csv
    largest_component=$(cat ${STD_OUT}| grep -w "largest_comp_bp" | cut -f2)
    python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa.components.csv ${largest_component} >>  "${SIM}_${STD_OUT}"    
done

```

## 3. Kmers labeling


```shell script

# Setting the reference cDBG unitigs fasta file
unitigs_fasta=aggressive_dislinked_cDBG_SRR11015356_k75

# Generating connected components CSV
python ${SCRIPTS}/unitigs_to_connected_components.py ${unitigs_fasta}.unitigs.fa

# Generating names file for the labeling process
python ${SCRIPTS}/unitigs_to_names_tsv.py ${unitigs_fasta}.unitigs.fa ${unitigs_fasta}.unitigs.fa.components.csv

# Start the kmers labeling
./cDBG_labeling ${unitigs_fasta}.unitigs.fa ${unitigs_fasta}.unitigs.fa.names.tsv ${unitigs_fasta}
```

## 4. Final components construction

### 4.1 Partitioning

```shell script
OUT_PREFIX=singlePartioning_aggressive_cDBG75
INDEX_PREFIX=aggressive_dislinked_cDBG_SRR11015356_k75
R1=SRR11015356_1.fasta
R2=SRR11015356_2.fasta

./single_primaryPartitioning ${INDEX_PREFIX} ${R1} ${R2} ${OUT_PREFIX}

```

### 4.1 Dumping

```shell script
SCRIPTS=omnigraph/scripts
THREADS=32

DB=singlePartioning_aggressive_cDBG75_omni.db
PAIRS_COUNT=singlePartioning_aggressive_cDBG75_pairsCount.tsv

python ${SCRIPTS}/dump_finalComps.py ${DB} ${PAIRS_COUNT} ${THREADS}

```

## 5. Assembly (Draft)

OUTPUT_DIR=
THREADS=
MAX_RAM=
READS=

/usr/bin/time -v rnaspades.py -s ${READS} -o ${OUTPUT_DIR} -m ${MAX_RAM} -t ${THREADS}


### 5.1 Assembly assessment (Draft)

THREADS=32
GTF=gencode.v34.chr_patch_hapl_scaff.annotation.gtf
REF_GENOME=GRCh38.p13.genome.fa
TRANSCRIPTS=transcripts.fasta

/usr/bin/time -v rnaQUAST.py --disable_infer_transcripts --disable_infer_genes --transcripts ${TRANSCRIPTS} --reference ${REF_GENOME} --gtf ${GTF} -t ${THREADS}

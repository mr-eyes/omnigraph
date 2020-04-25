# shellcheck disable=SC2188
<<PIPELINE

   ▄███████▄  ▄█     ▄███████▄    ▄████████  ▄█        ▄█  ███▄▄▄▄      ▄████████
  ███    ███ ███    ███    ███   ███    ███ ███       ███  ███▀▀▀██▄   ███    ███
  ███    ███ ███▌   ███    ███   ███    █▀  ███       ███▌ ███   ███   ███    █▀
  ███    ███ ███▌   ███    ███  ▄███▄▄▄     ███       ███▌ ███   ███  ▄███▄▄▄
▀█████████▀  ███▌ ▀█████████▀  ▀▀███▀▀▀     ███       ███▌ ███   ███ ▀▀███▀▀▀
  ███        ███    ███          ███    █▄  ███       ███  ███   ███   ███    █▄
  ███        ███    ███          ███    ███ ███▌    ▄ ███  ███   ███   ███    ███
 ▄████▀      █▀    ▄████▀        ██████████ █████▄▄██ █▀    ▀█   █▀    ██████████
                                            ▀
PIPELINE


wget -c https://sra-download.ncbi.nlm.nih.gov/traces/sra51/SRR/010757/SRR11015356 -O SRR11015356.sra

fastq-dump --fasta 0 --split-files SRR11015356.sra

# Creating a file with reads paths
ls -1 *fasta > list_reads

KMER_SIZE=75
MAX_MEMORY=60000
MIN_ABUNDANCE=1
FILTER_MODE=aggressive
ORIGINAL_cDBG_PREFIX=cDBG_SRR11015356_k75
OUTPUT_PREFIX=${ORIGINAL_cDBG_PREFIX}
SCRIPTS=scripts/ # scripts path in omnigraph/scripts
STD_OUT=stats_${FILTER_MODE}_${ORIGINAL_cDBG_PREFIX}.txt
THREADS=4

touch ${STD_OUT}

# Constructing cDBG
bcalm -kmer-size ${KMER_SIZE} -abundance-min ${MIN_ABUNDANCE} -nb-cores ${THREADS} -max-memory ${MAX_MEMORY} -out ${OUTPUT_PREFIX} -in list_reads

# Convert to GFA (Just for bandage)
python ${SCRIPTS}/unitigsToGFA.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.gfa ${KMER_SIZE} --single-directed


## Connected components
echo -e "Unitigs to connected components:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_connected_components.py ${OUTPUT_PREFIX}.unitigs.fa >> ${STD_OUT}
largest_component=$(cat ${STD_OUT} | grep -w "largest_comp_bp" | cut -f2)

## Getting LN&KM Stats
echo -e "LM & KM Stats:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv "${largest_component}" >>  ${STD_OUT}

## Edges Histogram
python ${SCRIPTS}/unitigs_edges_histogram.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv


## Filtration

OUTPUT_PREFIX=${FILTER_MODE}_dislinked_${ORIGINAL_cDBG_PREFIX}

python ${SCRIPTS}/unitigs_dislinkage.py ${ORIGINAL_cDBG_PREFIX}.unitigs.fa  ${KMER_SIZE} ${FILTER_MODE}

# Convert to GFA (Just for bandage)
python ${SCRIPTS}/unitigsToGFA.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.gfa ${KMER_SIZE} --single-directed



## Connected components
echo -e "Unitigs to connected components:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_connected_components.py ${OUTPUT_PREFIX}.unitigs.fa >> ${STD_OUT}
largest_component=$(cat ${STD_OUT}| grep -w "largest_comp_bp" | cut -f2)

## Getting LN&KM Stats
echo -e "LM & KM Stats:\n" >> ${STD_OUT}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv ${largest_component} >>  ${STD_OUT}

## Edges Histogram
python ${SCRIPTS}/unitigs_edges_histogram.py ${OUTPUT_PREFIX}.unitigs.fa ${OUTPUT_PREFIX}.unitigs.fa.components.csv



## Partitioning

# Setting the reference cDBG unitigs fasta file
unitigs_fasta=${OUTPUT_PREFIX}

# Generating connected components CSV
python ${SCRIPTS}/unitigs_to_connected_components.py ${unitigs_fasta}.unitigs.fa

# Generating names file for the labeling process
python ${SCRIPTS}/unitigs_to_names_tsv.py ${unitigs_fasta}.unitigs.fa ${unitigs_fasta}.unitigs.fa.components.csv

# Start the kmers labeling
./cDBG_labeling ${unitigs_fasta}.unitigs.fa ${unitigs_fasta}.unitigs.fa.names.tsv ${unitigs_fasta}


OUT_PREFIX=singlePartioning_aggressive_cDBG75
INDEX_PREFIX=${unitigs_fasta}
R1=SRR11015356_1.fasta
R2=SRR11015356_2.fasta

./single_primaryPartitioning ${INDEX_PREFIX} ${R1} ${R2} ${OUT_PREFIX}

DB=${OUT_PREFIX}_omni.db
PAIRS_COUNT=${OUT_PREFIX}_pairsCount.tsv

python ${SCRIPTS}/dump_finalComps.py ${DB} ${PAIRS_COUNT} ${THREADS}
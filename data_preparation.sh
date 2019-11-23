: '

1. Download the Human Transcriptome [REFERENCE]
2. Generate a names file
3. Indexing

'

ROOT_DIR=$(pwd)

mkdir -p dataset/reference

# Step 1
echo "Downloading ..."
cd dataset/reference
wget -c ftp://ftp.ebi.ac.uk/pub/databases/gencode/Gencode_human/release_32/gencode.v32.transcripts.fa.gz

# Step 2
echo "Generating names file"
zcat gencode.v32.transcripts.fa.gz | grep ">" | cut -c 2- | awk 'BEGIN{FS=OFS="\t";}{print $1,$1}' | sort | uniq > gencode.v32.transcripts.fa.names


# Step 3
echo "Indexing ..."
cd ${ROOT_DIR}
mkdir -p index
python indexing.py ${ROOT_DIR}/dataset/reference/gencode.v32.transcripts.fa.gz ${ROOT_DIR}/dataset/reference/gencode.v32.transcripts.fa.names
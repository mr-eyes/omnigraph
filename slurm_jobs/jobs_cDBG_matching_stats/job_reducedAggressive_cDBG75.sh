#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J reduced_985_aggressive_dislinked_cDBG75
#SBATCH --time=10:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 1
#SBATCH --mem=20gb
#SBATCH --output=slurm_%x.%j.out
#SBATCH --error=slurm_%x.%j.err


# activate conda in general (MUST BE PLACE BEFORE ANYTHING)
. /home/mhussien/miniconda3/etc/profile.d/conda.sh
conda activate omnigraph


# exit when any command fails
set -e
set -o nounset
set -o errexit
set -x
set -euox pipefail


# make a directory specific to user and job
export MYTMP=/scratch/${USER}/slurm_${SLURM_JOB_NAME}_${SLURM_JOBID}
mkdir -p "$MYTMP"

# force clean it up
function cleanup() {
    rm -rf "$MYTMP";
}

trap cleanup EXIT

# ~~~~~~~~~~~~~~~~~~~~~ START Job ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cd ${MYTMP}

# Files names
original=cDBG_SRR11015356_k75
mild=mild_dislinked_cDBG_SRR11015356_k75
aggressive=aggressive_dislinked_cDBG_SRR11015356_k75
reduced_original=original_reduced_985_cDBG75
reduced_mild=reduced_985_mild_dislinked_cDBG75
reduced_aggressive=reduced_985_aggressive_dislinked_cDBG75

INDEX_PREFIX=${reduced_aggressive}

# Copy required files from the original directory to the scratch directory

## Copy SRR Reads
cp /home/mhussien/omnigraph/data/SRR11015356_1.fasta ./
cp /home/mhussien/omnigraph/data/SRR11015356_2.fasta ./

# Set Global Variables
SCRIPTS=/home/mhussien/omnigraph/scripts
THREADS=1
MAX_RAM_MB=20000
MAX_RAM_GB=20
K_SIZE=75

# Won't make a big difference if copied to scratch
INDEXES_DIR=/home/mhussien/omnigraph/data/cDBG75/six_gfas/indexes_sixGFAs_labeling_CDBG75


############################## (1) START Partitioning ####################################

# run: ./primaryPartitioning <index_prefix> <PE_R1> <PE_R2> <out_prefix>

/usr/bin/time -v /home/mhussien/omnigraph/build/primaryPartitioning ${INDEXES_DIR}/${INDEX_PREFIX} SRR11015356_1.fasta SRR11015356_2.fasta ${INDEX_PREFIX}

############################## DONE Labeling #######################################

rm -rf SRR11015356_1.fasta
rm -rf SRR11015356_2.fasta

########################### Move scratch to home dir ###################

cp -r $MYTMP $SLURM_SUBMIT_DIR
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J kmersColoring_originalCDBG75
#SBATCH --time=1:00:00
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

cd "$MYTMP"

# PLACEHOLDERS | REPLACE
REF_FASTA=cDBG_SRR11015356_k75.unitigs.fa
NAMES_TSV=cDBG_SRR11015356_k75.unitigs.fa.names

# Copy required files from the original directory to the scratch directory
cp /home/mhussien/omnigraph/data/cDBG75/${REF_FASTA} ./
cp /home/mhussien/omnigraph/data/cDBG75/${NAMES_TSV} ./

# Set Global Variables
SCRIPTS=/home/mhussien/omnigraph/scripts
THREADS=1
MAX_RAM_MB=20000
MAX_RAM_GB=20
K_SIZE=75

############################## (1) START CDHIT ####################################

OUT_PREFIX=kmersColoring_original_cDBG75

/usr/bin/time -v /home/mhussien/omnigraph/build/kmersColoring ${REF_FASTA} ${NAMES_TSV} ${OUT_PREFIX}

############################## DONE Coloring #######################################

########################### Move scratch to home dir ###################

rm -rf ${REF_FASTA}

cp -r $MYTMP $SLURM_SUBMIT_DIR
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
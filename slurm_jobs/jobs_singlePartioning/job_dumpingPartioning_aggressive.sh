#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J singlePartioning_aggressive
#SBATCH --time=5:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 32
#SBATCH --mem=60gb
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

## Copy paritions db
cp /home/mhussien/omnigraph/data/cDBG75/singlePartitions/slurm/singlePart_aggressive/slurm_singlePartioning_aggressive_20550930/singlePartioning_aggressive_cDBG75_omni.db ./
cp /home/mhussien/omnigraph/data/cDBG75/singlePartitions/slurm/singlePart_aggressive/slurm_singlePartioning_aggressive_20550930/singlePartioning_aggressive_cDBG75_pairsCount.tsv ./

# Set Global Variables
SCRIPTS=/home/mhussien/omnigraph/scripts
THREADS=32

############################## (1) START Dumping ####################################

DB=singlePartioning_aggressive_cDBG75_omni.db
PAIRS_COUNT=singlePartioning_aggressive_cDBG75_pairsCount.tsv

/usr/bin/time -v python ${SCRIPTS}/dump_finalComps.py ${DB} ${PAIRS_COUNT} ${THREADS}

# Remove reads
rm -rf ${DB}
rm -rf ${PAIRS_COUNT}

############################## DONE Partitioning #######################################


########################### Move scratch to home dir ###################


cp -r $MYTMP "$SLURM_SUBMIT_DIR"
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
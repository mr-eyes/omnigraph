#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmm
#SBATCH -J singlePartioning_reducedAggressive
#SBATCH --time=10:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 1
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

## Copy SRR Reads
cp /home/mhussien/omnigraph/data/SRR11015356_1.fasta ./
cp /home/mhussien/omnigraph/data/SRR11015356_2.fasta ./


############################## (1) START Partioning ####################################

OUT_PREFIX=singlePartioning_reducedAggressive_cDBG75
INDEX_PREFIX=/home/mhussien/omnigraph/data/cDBG75/six_gfas/indexes_sixGFAs_labeling_CDBG75/reduced_985_aggressive_dislinked_cDBG75
R1=SRR11015356_1.fasta
R2=SRR11015356_2.fasta

/usr/bin/time -v /home/mhussien/omnigraph/build/single_primaryPartitioning ${INDEX_PREFIX} ${R1} ${R2} ${OUT_PREFIX}

# Remove reads
rm -rf ${R1}
rm -rf ${R2}

############################## DONE Partitioning #######################################


########################### Move scratch to home dir ###################


cp -r $MYTMP "$SLURM_SUBMIT_DIR"
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
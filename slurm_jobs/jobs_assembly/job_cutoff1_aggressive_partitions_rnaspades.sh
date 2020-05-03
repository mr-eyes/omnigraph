#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J assembly_cutoff1_aggressive_partitions_rnaspades
#SBATCH --time=20:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 32
#SBATCH --mem=24gb
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
  cp -r $MYTMP "$SLURM_SUBMIT_DIR"
  rm -rf "$MYTMP"
}

trap cleanup EXIT

# ~~~~~~~~~~~~~~~~~~~~~ START Job ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cd "$MYTMP"

## Copy paritions fasta files directory
cp -r "/home/mhussien/omnigraph/data/cDBG75/singlePartitions/slurm/partitions/slurm_dumpingPartions_aggressive_20763906/dumped_partitions_cutoff1_singlePartioning_aggressive_cDBG75_omni" ./all_fa

# Set Global Variables
THREADS=32

############################## (1) START assembly ####################################

for file in all_fa/*fa; do
  echo "Assembling: $file"
  rnaspades.py -s "${file}" -o _rnaspades_"${file}" -m 24 -t ${THREADS}
done

############################## DONE Partitioning #######################################

########################### Move scratch to home dir ###################

cp -r $MYTMP "$SLURM_SUBMIT_DIR"
scontrol show job $SLURM_JOB_ID # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID} # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch

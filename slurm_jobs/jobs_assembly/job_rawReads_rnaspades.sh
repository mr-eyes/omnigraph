#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J assembly_rawReads_rnaSpades
#SBATCH --time=10:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 20
#SBATCH --mem=150gb
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
    rm -rf "$MYTMP";
}

trap cleanup EXIT

# ~~~~~~~~~~~~~~~~~~~~~ START Job ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cd "$MYTMP"

## Copy SRR Reads
cp /home/mhussien/omnigraph/data/SRR11015356_1.fasta ./
cp /home/mhussien/omnigraph/data/SRR11015356_2.fasta ./

# Set Global Variables
THREADS=20
OUTPUT_DIR=_rnaspades_rawReads_


############################## (1) START Dumping ####################################

/usr/bin/time -v rnaspades.py -1 SRR11015356_1.fasta -2 SRR11015356_2.fasta -o ${OUTPUT_DIR} -m 150 -t ${THREADS}

############################## DONE Partitioning #######################################

rm -rf SRR11015356_1.fasta
rm -rf SRR11015356_2.fasta

########################### Move scratch to home dir ###################


cp -r $MYTMP "$SLURM_SUBMIT_DIR"
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch

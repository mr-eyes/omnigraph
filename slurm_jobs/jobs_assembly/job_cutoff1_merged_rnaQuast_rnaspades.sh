#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J merged_cutoff1_trinity
#SBATCH --time=10:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 32
#SBATCH --mem=80gb
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
  rm -rf "$MYTMP"
}

trap cleanup EXIT

# ~~~~~~~~~~~~~~~~~~~~~ START Job ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cd "$MYTMP"

## Copy reference genome db
cp /home/mhussien/omnigraph/data/reference_genome/* ./

## Copy rnaspades assembled transcripts
cp /home/mhussien/omnigraph/data/cDBG75/singlePartitions/slurm/assembly/merged_cutoff1_rnaspades/slurm_merged_cutoff1_rnaspades_20737449/_rnaspades_cutoff_1_/transcripts.fasta ./

# Set Global Variables
THREADS=32
GTF=gencode.v34.chr_patch_hapl_scaff.annotation.gtf
REF_GENOME=GRCh38.p13.genome.fa
TRANSCRIPTS=transcripts.fasta

############################## (1) START Dumping ####################################

/usr/bin/time -v rnaQUAST.py --transcripts ${TRANSCRIPTS} --reference ${REF_GENOME} --gtf ${GTF} -t ${THREADS} disable_infer_transcripts=True

############################## DONE Partitioning #######################################

########################### Move scratch to home dir ###################

cp -r $MYTMP "$SLURM_SUBMIT_DIR"
scontrol show job $SLURM_JOB_ID # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID} # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch

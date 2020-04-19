#!/usr/bin/env bash
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J sixGFAs_labeling_CDBG75
#SBATCH --time=3:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 1
#SBATCH --mem=40gb
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

# Files names
original=cDBG_SRR11015356_k75
mild=mild_dislinked_cDBG_SRR11015356_k75
aggressive=aggressive_dislinked_cDBG_SRR11015356_k75
reduced_original=original_reduced_985_cDBG75
reduced_mild=reduced_985_mild_dislinked_cDBG75
reduced_aggressive=reduced_985_aggressive_dislinked_cDBG75

# Copy required files from the original directory to the scratch directory

for unitigs_fasta in ${reduced_original} ${reduced_mild} ${reduced_aggressive} ${original} ${mild} ${aggressive} ;
do
    echo -e "copying $unitigs_fasta ... \n";
    cp /home/mhussien/omnigraph/data/cDBG75/six_gfas/${unitigs_fasta}.unitigs.fa ./;
    cp /home/mhussien/omnigraph/data/cDBG75/six_gfas/${unitigs_fasta}.unitigs.fa.names.tsv ./;
done

# Set Global Variables
SCRIPTS=/home/mhussien/omnigraph/scripts
THREADS=1
MAX_RAM_MB=40000
MAX_RAM_GB=40
K_SIZE=75

############################## (1) START Labeling ####################################

for unitigs_fasta in ${reduced_original} ${reduced_mild} ${reduced_aggressive} ${original} ${mild} ${aggressive} ;
do
    echo -e "labeling $unitigs_fasta ... \n";
    python ${SCRIPTS}/unitigs_to_connected_components.py ${unitigs_fasta}.unitigs.fa
    python ${SCRIPTS}/unitigs_to_names_tsv.py ${unitigs_fasta}.unitigs.fa ${unitigs_fasta}.unitigs.fa.components.csv
    /usr/bin/time -v /home/mhussien/omnigraph/build/cDBG_labeling ${unitigs_fasta}.unitigs.fa ${unitigs_fasta}.unitigs.fa.names.tsv ${unitigs_fasta}
    rm -rf ${unitigs_fasta}.unitigs.fa
    rm -rf ${unitigs_fasta}.unitigs.fa.names.tsv
done

############################## DONE Labeling #######################################

########################### Move scratch to home dir ###################

cp -r $MYTMP $SLURM_SUBMIT_DIR
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
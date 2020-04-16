#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J missing_stats
#SBATCH --time=4:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 5
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

# Set Global Variables
SCRIPTS=/home/mhussien/omnigraph/scripts

cd "$MYTMP"

ORIGINAL_FASTA=cDBG_SRR11015356_k75.unitigs.fa
MILD_FASTA=mild_dislinked_cDBG_SRR11015356_k75.unitigs.fa
AGGRESSIVE_FASTA=aggressive_dislinked_cDBG_SRR11015356_k75.unitigs.fa


# Copy required files from the original directory to the scratch directory
cp /home/mhussien/omnigraph/data/cDBG75/${ORIGINAL_FASTA} ./
cp /home/mhussien/omnigraph/data/cDBG75/${MILD_FASTA} ./
cp /home/mhussien/omnigraph/data/cDBG75/${AGGRESSIVE_FASTA} ./


# Original
echo  "---------------------------   ORIGINAL    -----------------------------------------"
python ${SCRIPTS}/unitigs_to_connected_components.py ${ORIGINAL_FASTA}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${ORIGINAL_FASTA} ${ORIGINAL_FASTA}.components.csv 8 > original_ln_km.txt
python ${SCRIPTS}/unitigsToGFA.py ${ORIGINAL_FASTA} ${ORIGINAL_FASTA}.gfa 75 --single-directed
Bandage info ${ORIGINAL_FASTA}.gfa &> original_bandage_info.txt


# MILD_FASTA
echo  "---------------------------   MILD    -----------------------------------------"
python ${SCRIPTS}/unitigs_to_connected_components.py ${MILD_FASTA}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${MILD_FASTA} ${MILD_FASTA}.components.csv 8 > mild_ln_km.txt


# AGGRESSIVE_FASTA
echo  "---------------------------   ORIGINAL    -----------------------------------------"
python ${SCRIPTS}/unitigs_to_connected_components.py ${AGGRESSIVE_FASTA}
python ${SCRIPTS}/unitigs_to_viz_kmersHistograms.py ${AGGRESSIVE_FASTA} ${AGGRESSIVE_FASTA}.components.csv 8 > aggressive_ln_km.txt


# ~~~~~~~~~~~~~~~~~~~~~ END Job ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


########################### Move scratch to home dir ###################

rm ${ORIGINAL_FASTA} ${MILD_FASTA} ${AGGRESSIVE_FASTA}

cp -r ${MYTMP} ${SLURM_SUBMIT_DIR}
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J cDBG75_0.985_Analysis_filtered_dislinkage
#SBATCH --time=4:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 10
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
    rm -rf "$MYTMP";
}

trap cleanup EXIT

# ~~~~~~~~~~~~~~~~~~~~~ START Job ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cd "$MYTMP"

REF_FASTA=filtered_dislinked_cDBG_SRR11015356_k75.unitigs.fa

# Copy required files from the original directory to the scratch directory
cp /home/mhussien/omnigraph/data/cDBG75/$REF_FASTA ./


# Set Global Variables
SCRIPTS=/home/mhussien/omnigraph/scripts
THREADS=10
OUT_PREFIX=cDBG75
MAX_RAM_MB=64000
MAX_RAM_GB=64
K_SIZE=75

############################## (1) START CDHIT ####################################

echo "CD-HIT on %985 ..."
WORD_SIZE=11
SIM=0.985
cd-hit-est -i ${REF_FASTA} -n ${WORD_SIZE} -c ${SIM} -o clusters_${SIM}_${OUT_PREFIX} -d 0 -T ${THREADS} -M ${MAX_RAM_MB}  # &> log_cdhit_${SIM}_${OUT_PREFIX}.log

################################ END CDHIT ######################################


##################### (2) EXTRACTING Representatives from REF_FASTA ##################

for SIM in 0.985
do
    echo "Extracting representatives from clusters at %${SIM}"
    cat clusters_${SIM}_${OUT_PREFIX}.clstr | grep "\*" | awk -F"[>.]" '{print ">"$2}' | grep -Fwf - -A1 <(seqkit seq -w 0 ${REF_FASTA}) | grep -v "^\-\-" > reps_unitigs_${OUT_PREFIX}_${SIM}.fa
done

########################## DONE EXTRACTION Representatives ########################


############# (3) Constructing cDBG of the representative sequences ##################

for SIM in 0.985
do
    echo "Constructing cDBG of representatives at %${SIM}"
    bcalm -kmer-size ${K_SIZE} -nb-cores ${THREADS} -max-memory ${MAX_RAM_MB} -abundance-min 1 -out cDBG2_${SIM}_${OUT_PREFIX} -in reps_unitigs_${OUT_PREFIX}_${SIM}.fa # &> log_bcalm_cDBG_reps_unitigs_${OUT_PREFIX}_${SIM}
    echo "Converting reps_unitigs_${OUT_PREFIX}_${SIM}.fa to GFA"
    python ${SCRIPTS}/unitigsToGFA.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.gfa ${K_SIZE} --single-directed
    echo "Getting cDBG Stats for cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa"
    Bandage info cDBG2_${SIM}_${OUT_PREFIX}.unitigs.gfa # &> log_Bandage_cDBG2_${SIM}_${OUT_PREFIX}.gfa.log
    python ${SCRIPTS}/unitigs_to_connected_components.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa  # &> log_script_connectedComponents_cDBG2_${SIM}_${OUT_PREFIX}.gfa.log
    python ${SCRIPTS}/unitigs_edges_histogram.py cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa cDBG2_${SIM}_${OUT_PREFIX}.unitigs.fa.components.csv
done
##################### DONE EXTRACTION Representatives ##################

########################### Move scratch to home dir ###################

rm -rf ${REF_FASTA}

cp -r $MYTMP $SLURM_SUBMIT_DIR
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
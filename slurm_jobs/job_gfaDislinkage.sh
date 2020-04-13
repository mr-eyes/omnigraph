#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J gfaDislinkage
#SBATCH --time=3:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 1
#SBATCH --mem=50gb
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
mkdir -p $MYTMP

# force clean it up
function cleanup() {
    rm -rf $MYTMP;
}

trap cleanup EXIT

# <run your code, telling it to use that dir;

cd $MYTMP

touch ${MYTMP}/scratch_fullData_firstQuery.log
echo "running in $(pwd)" > ${MYTMP}/scratch_fullData_firstQuery.log


which python
python --version


cp -r /home/mhussien/omnigraph/kExplorer/graph_analysis ./
cp /home/mhussien/omnigraph/data/cDBG_SRR11015356_k31.unitigs.gfa ./

ls

/usr/bin/time -v python graph_analysis/1-dislinkage.py cDBG_SRR11015356_k31.unitigs.gfa 31

rm cDBG_SRR11015356_k31.unitigs.gfa
rm -rf graph_analysis


cp -r $MYTMP $SLURM_SUBMIT_DIR
scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
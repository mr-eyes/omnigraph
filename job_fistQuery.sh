#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J firstQuery
#SBATCH --time=15:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 1
#SBATCH --mem=16gb
#SBATCH --output=slurm_%x.%j.out
#SBATCH --error=slurm_%x.%j.out

# exit when any command fails
set -e
set -o nounset
set -o errexit
set -x


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

/usr/bin/time -v /home/mhussien/omnigraph/build/query_1 --db ${MYTMP}/query_1.db #&> ${MYTMP}/scratch_fullData_firstQuery.log

cp -r $MYTMP $SLURM_SUBMIT_DIR

scontrol show job $SLURM_JOB_ID     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

sstat --format 'JobID,MaxRSS,AveCPU' -P ${SLURM_JOB_ID}.batch
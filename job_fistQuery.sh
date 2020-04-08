#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bmh
#SBATCH -J sgc
#SBATCH --time=15:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 1
#SBATCH --mem=60gb

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT

# activate conda in general
. "/home/mhussien/miniconda3/etc/profile.d/conda.sh"

conda activate omnigraph

cd /home/mhussien/omnigraph/build

# make a directory specific to user and job
export MYTMP=/scratch/${USER}/slurm_${SLURM_JOBID}
mkdir -p ${MYTMP}

# force clean it up
function cleanup() { rm -rf $MYTMP; }
trap cleanup EXIT

# <run your code, telling it to use that dir;
echo running in $(pwd)

/usr/bin/time -v ./query_1 ${MYTMP}/scratch_query1_result.db &> /home/mhussien/omnigraph/logs/fullData_firstQuery.log
cp ${MYTMP}/scratch_query1_result.db ${SLURM_SUBMIT_DIR}

scontrol show job ${SLURM_JOB_ID}     # Print out final statistics about resource uses before job exits

# Print out values of the current jobs SLURM environment variables
env | grep SLURM
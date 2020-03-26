#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bml
#SBATCH -J sgc
#SBATCH --time=05:00:00
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

/usr/bin/time -v ./query_1 &> /home/mhussien/omnigraph/logs/firstQuery.log

# Print out values of the current jobs SLURM environment variables
env | grep SLURM
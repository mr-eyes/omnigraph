#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mabuelanin@gmail.com
#SBATCH -p bml
#SBATCH -J sgc
#SBATCH --time=00:30:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 1
#SBATCH --mem=15gb

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT

# activate conda in general
. "/home/mhussien/miniconda3/etc/profile.d/conda.sh"

conda activate omnigraph

cd /home/mhussien/omnigraph

/usr/bin/time -v python scripts/collectiveComps_indexing.py data/cDBG_SRR11015356_k31.unitigs.fa &> kProcessor_fist_indexing.log


# Print out values of the current jobs SLURM environment variables
env | grep SLURM
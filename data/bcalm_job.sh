#! /bin/bash -login
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mahussien@ucdavis.edu
#SBATCH -p bml
#SBATCH -J sgc
#SBATCH --time=03:00:00
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 12
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

cd /home/mhussien/omnigraph/data

/usr/bin/time -v bcalm -kmer-size 31 -abundance-min 3 -max-memory 60000 -nb-cores 12 -out cDBG_SRR11015356_k31 -in list_reads &> 1.1_cDBG_log.txt

# Print out values of the current jobs SLURM environment variables
env | grep SLURM
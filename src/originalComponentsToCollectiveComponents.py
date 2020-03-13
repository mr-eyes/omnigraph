"""
This script is used under the project "Universal Transcriptome"

Input: CSV File with the following format:
    A. Column(1): Original Component ID
    B. Column(3:): Unitigs ID(s)

Output: CS File with the following format:
    A. Column(1): Original Component ID
    B. Column(2): Collective component ID
    C. Column(3:): Unitigs ID(s)

Run:
python originalComponentsToCollectiveComponents.py <originalCSV> <no_of_collective_components>

Example:
    python originalComponentsToCollectiveComponents.py file.csv 50000    

"""

import sys
import subprocess
from tqdm import tqdm
import os

def compress(original, max_range):
    """
    This function performs mapping from short list to a longer list using repeats.
    original: Long list of values
    max_range: The maximum range of the new list.
    """

    result = dict()
    step = 0
    factor = len(original) // max_range
    for new in range(1, max_range + 1 , 1):
        for i in original[step:step + factor]:
            result[i] = new
        step += factor

    else:
        for i in original[step:]:
            result[i] = new
    
    return result


if len(sys.argv) < 3:
    sys.exit("run: python originalComponentsToCollectiveComponents.py <originalCSV> <no_of_collective_components>")

original_csv = sys.argv[1]
collectiveCompNo = int(sys.argv[2])
outputFile = "collectiveComps_" + os.path.basename(original_csv)

cmd = f"tac {original_csv} | head -n 1 | grep -E -o '[[:digit:]]+' | head -1"
originalCompsNo = int(subprocess.getoutput(cmd).split()[0])

originalToCollective = compress(list(range(1, originalCompsNo + 1)), collectiveCompNo)

with open(original_csv, 'r') as componentsReader, open(outputFile, 'w') as outputWriter:
    for line in tqdm(componentsReader, total=originalCompsNo):
        line = line.strip().split(',')
        original_compID = int(line[0])
        all_unitigs = line[1:]
        collective_component = originalToCollective[original_compID]
        new_list = [str(original_compID)] + [str(collective_component)] + all_unitigs
        new_line = ','.join(new_list) + '\n'
        outputWriter.write(new_line)

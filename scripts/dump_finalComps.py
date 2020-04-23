"""
This script is used under the project "Omnigraph"

Input:
    1. sqlite DB File
    2. PairsCount TSV file generated by the executable: single_primaryPartitioning.
    3. cutoff threshold, filter out edges with weight < cutoff.

Output:
    1. Directory of fasta files, each file contains a final component sequences.
    2. TSV with col1:finalCompID col2:originalComponentsIDs


Run:
python dump_finalComps.py <db_file> <pairsCountFile> <optional: cutoff (default: 1)>
"""

import sqlite3
import sys
from collections import defaultdict
import os
from tqdm import tqdm


class FastaWriter:

    def __init__(self, output_dir, number_of_files):
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        else:
            raise ValueError("Cannot create output directory")

        self.fastaWriters = dict()
        for finalComp_id in range(1, number_of_files + 1):
            file_name = os.path.join(output_dir, str(finalComp_id) + ".fa")
            self.fastaWriters[finalComp_id] = open(file_name, 'w')

    def write_seq(self, _finalCompID, PE, R_ID, seq):
        self.fastaWriters[_finalCompID].write(f">{PE}.{R_ID}\t{_finalCompID}\n{seq}\n")

    def close_all(self):
        for _, fileHandler in self.fastaWriters:
            fileHandler.close()


class SQLiteReader:

    def __init__(self, sqlite_file):
        try:
            self.conn = sqlite3.connect(sqlite_file)

        except sqlite3.Error as err:
            print(err)
            _err_msg = f"could not connect to {sqlite_file}"
            raise ValueError(_err_msg)

        self.sqlite_file = sqlite_file

    def get_table_data(self, table_name):
        curs = self.conn.execute(f'select * from {table_name}')
        rows = curs.fetchall()
        return rows

    def get_rows_curs(self, table_name):
        curs = self.conn.execute(f'select * from {table_name}')
        return curs

    def get_num_rows(self):
        curs = self.conn.execute("select seq from sqlite_sequence where name = 'reads'")
        return curs.fetchall()[0][0]


class ConnectedComponents:

    def __init__(self, min_count=1):
        self.__source = list()
        self.__target = list()
        self.final_components = dict()
        self.number_of_components = int()
        self.THRESHOLD = min_count

    def add_edge(self, source_node, target_node, pair_count):
        if pair_count >= self.THRESHOLD:
            self.__source.append(source_node)
            self.__target.append(target_node)

    def construct(self):
        __leaders = defaultdict(lambda: None)
        __groups = defaultdict(set)

        def __find(x):
            l = __leaders[x]
            if l is not None:
                l = __find(l)
                __leaders[x] = l
                return l
            return x

        def __union(x, y):
            lx, ly = __find(x), __find(y)
            if lx != ly:
                __leaders[lx] = ly

        for i in range(len(self.__source)):
            __union(self.__source[i], self.__target[i])

        for x in __leaders:
            __groups[__find(x)].add(x)

        for component_id, (k, v) in enumerate(__groups.items(), start=1):
            self.final_components[component_id] = v

        self.number_of_components = len(self.final_components)

    def get_components_dict(self):
        return self.final_components

    def dump_to_tsv(self, file_name):
        with open(file_name, 'w') as tsvWriter:
            for compID, nodes in self.final_components.items():
                nodes = ','.join(map(str, nodes))
                tsvWriter.write(f"{compID}\t{nodes}\n")

    def __del__(self):
        del self


def get_nodes_sizes(components_file_path):
    """
    Return the size of each original component so we can apply threshold on weight.
    :param components_file_path:
    :return: node_to_size
    """
    node_to_size = dict()
    with open(components_file_path, 'r') as compsReader:
        for _line in compsReader:
            _line = list(map(int, _line.strip().split(',')))
            _compID = int(_line[0])
            node_to_size[_compID] = len(_line) - 1

    return node_to_size


if len(sys.argv) < 3:
    raise ValueError("run: python dump_finalComps.py dump_finalComps.py <db_file> <pairsCountFile> <cutoff>")

cutoff_threshold = 1
sqlite_db_path = sys.argv[1]
pairsCountFile = sys.argv[2]

if len(sys.argv) == 4:
    cutoff_threshold = int(sys.argv[3])

"""
1. Parse the pairsCount to edges
"""

edges = []
with open(pairsCountFile, 'r') as pairsCountReader:
    next(pairsCountReader)  # skip header
    for line in pairsCountReader:
        edges.append(tuple(map(int, line.strip().split())))

"""
2. Construct final components
"""

components = ConnectedComponents(min_count=cutoff_threshold)
for edge in edges:
    components.add_edge(*edge)

components.construct()
number_of_final_components = components.number_of_components

print(f"Number of connected comps: {number_of_final_components}")

# Map old original component node to new final component
originalComp_to_finalComp = dict()
for finalCompID, originalComps in components.get_components_dict().items():
    for originalCompID in originalComps:
        originalComp_to_finalComp[originalCompID] = finalCompID

"""
3. Create fastaWriter instance 
"""

fastaWriter = FastaWriter(output_dir=os.path.basename(sqlite_db_path), number_of_files=number_of_final_components)

"""
4. Dump the partitions
"""

dbReader = SQLiteReader(sqlite_db_path)

for row in tqdm(dbReader.get_rows_curs("reads"), total=dbReader.get_num_rows()):
    ID, R1_seq, R2_seq, R1_originalComp, R2_originalComp = row
    if R1_originalComp in originalComp_to_finalComp and R2_originalComp in originalComp_to_finalComp:
        fastaWriter.write_seq(_finalCompID=originalComp_to_finalComp[R1_originalComp], PE=1, R_ID=ID, seq=R1_seq)
        fastaWriter.write_seq(_finalCompID=originalComp_to_finalComp[R2_originalComp], PE=2, R_ID=ID, seq=R2_seq)

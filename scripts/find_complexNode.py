import networkx as nx
import sys

if len(sys.argv) < 2:
    exit("pass unitigs_file.fa")

unitigs_file = sys.argv[1]


def get_nodeID(link):
    return int(link[4:-2])


edges = list()

with open(unitigs_file, 'r') as unitigsReader:
    for line in unitigsReader:
        header = line.strip().split()
        unitig_id = int(header[0][1:])
        links = list(map(get_nodeID, header[4:]))
        for link in links:
            edges.append([unitig_id, link])

        next(unitigsReader)

G = nx.Graph()
G.add_edges_from(edges)
complex_node = max(dict(G.degree()).items(), key=lambda x: x[1])

print(complex_node)

import sys
import gzip
import plotly.graph_objs as go
from plotly.offline import plot
import subprocess
from tqdm import tqdm
import os


def get_lines_count(file_path):
    if file_path[-3:] == ".gz":
        ps = subprocess.Popen(f"gzip -cd {file_path} | wc -l", shell=True, stdout=subprocess.PIPE,
                              stderr=subprocess.STDOUT)
        return int(ps.communicate()[0])

    else:
        return int(subprocess.getoutput('wc -l ' + file_path).split()[0])


def auto_open(file_name, mode, encoding='utf8'):
    if file_name[-3:] == ".gz":
        return gzip.open(file_name, mode + 't', encoding=encoding)
    else:
        return open(file_name, mode)


file_names = sys.argv[1:]

filesData = dict()

STEP = 10
ranges = [(start, start + STEP) for start in range(0, 100, STEP)]

for file in file_names:
    filesData[file] = dict()
    for R in ["R1", "R2"]:
        filesData[file][R] = {limit_range: 0 for limit_range in ranges}
        filesData[file][R].update({0: 0, 100: 0})

for file in file_names:
    print(f"Processing: {os.path.basename(file)}")
    line_count = get_lines_count(file) - 1

    with auto_open(file, 'r') as TSV_Reader:
        next(TSV_Reader)  # skip header
        for line in tqdm(TSV_Reader, total=line_count):
            line = line.split('\t')
            R = 'R' + line[1]
            percentage = float(line[2]) * 100

            _found_single_flat = False

            for single_value in [0, 100]:
                if percentage == single_value:
                    filesData[file][R][single_value] += 1
                    _found_single_flat = True

            if not _found_single_flat:
                for start, end in ranges:
                    if start < percentage <= end:
                        filesData[file][R][(start, end)] += 1
                        break

# For debugging
# for file, fragment in filesData.items():
#     print(f"file: {file}:")
#     for R, limit_ranges in fragment.items():
#         print(f"\t{R}")
#         for limit_range, count in limit_ranges.items():
#             print(f"\t\t{limit_range}: {count}")

fig = go.Figure()

for file, fragment in filesData.items():
    for R, limit_ranges in fragment.items():
        fig.add_trace(go.Bar(name=f'{R}-{os.path.basename(file)}', x=list(map(str, limit_ranges.keys())),
                             y=list(limit_ranges.values())))

fig.update_layout(barmode='group', yaxis_type="log")
plot(fig, filename='six_gfas_stats.html', auto_open=False)

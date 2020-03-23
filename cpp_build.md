# Querying

## Cloning & building

```bash
THIS=$(pwd)
cd cpp_exp/
git clone git@github.com:dib-lab/kProcessor.git
cd kProcessor
git checkout development
git submodule update --init --recursive
cd ${THIS}
mkdir build
cd build
cmake ..
make
```

## Run

```bash
cd cpp_exp/build
./query_1 reps_unitigs_SRR11015356_k75.fa <index_prefix> <reads_file>
```

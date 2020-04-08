# Robust Content-and-Structure (RCAS) Index

This is the code for the RCAS index proposed in our paper "Dynamic Interleaving
of Content and Structure for Robust Indexing of Semi-Structured Hierarchical
Data".


## Compilation

The code is written in C++11. You need
- A C++11 compliant compiler
- CMake


### Initializing Required Libraries

Our code uses the B+ Tree from the [TLX Library](https://github.com/tlx/tlx) in
our experimental evaluation. Include the TLX library by initializing the git
submodules:

```
git submodule init
git submodule update
```


### Compiling in DEBUG Mode:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```


### Compiling in RELEASE Mode:

Compiling in RELEASE mode turns on optimizations:

```
mkdir release
cd release
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Executing the Code

### Running Example

The RCAS index on the BOM data from our running example can be created and
printed as follows:

```
cd build
make
./app
```

### Tests

A few test cases exist and can be executed as follows

```
cd build
make
./tests/castest
```


## Experiments

### Datasets

We published the datasets used in this paper on [Zenodo](https://doi.org/10.5281/zenodo.3739263).
Download the datasets in a folder `/path/to/datasets` as follows:

```
cd /path/to/datasets
wget "https://zenodo.org/record/3739263/files/sf_dataset.tar.gz?download=1"
wget "https://zenodo.org/record/3739263/files/amazon.tar.gz?download=1"
wget "https://zenodo.org/record/3739263/files/xmark.tar.gz?download=1"
tar -xvf sf_dataset.tar.gz
tar -xvf amazon.tar.gz
tar -xvf xmark.tar.gz
```

### Executing the Experiments

The experiments can be executed with the following shell script. Provide the
path to the datasets `/path/to/datasets` from the step before as a parameter.

```
./scripts/run_benchmarks.sh /path/to/datasets
```

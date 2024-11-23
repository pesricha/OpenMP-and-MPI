# OpenMP-and-MPI-Prefix-Sum

# Parallel Programming with OpenMP and MPI

This project demonstrates two approaches to parallel programming: **OpenMP** for shared memory systems and **MPI** for distributed memory systems. The implementations focus on efficient parallel computations for prefix sum and target searching within an array.

---

## Features

### OpenMP Implementation
- **Function**: `prefixSumOMP`
- Performs prefix sum computation on a shared memory system.
- Key highlights:
  - **Thread-Level Parallelism**: Splits the array into chunks, assigning each to a thread.
  - **Synchronization**: Utilizes barriers to synchronize thread execution.
  - **SIMD Optimization**: Applies SIMD directives to enhance performance during adjustment phases.

### MPI Implementation
- **Functionality**: Distributed search for a target element in an array.
- Key highlights:
  - **Data Distribution**: Uses `MPI_Scatter` to distribute data among processes.
  - **Broadcasting**: Uses `MPI_Bcast` to share the target across all processes.
  - **Reduction Operation**: Uses `MPI_Allreduce` to find the global result across ranks.
  - **Timing**: Measures and reports execution time for performance analysis.

---

## Setup

### Prerequisites
- **Compiler**: Requires a C++ compiler with OpenMP and MPI support (e.g., GCC or Intel C++ Compiler).
- **Libraries**: MPI library (e.g., MPICH or OpenMPI).

### Compilation
- For OpenMP:
  ```bash
  cd openmp/
  g++ -fopenmp -o ompprefixsum main.cpp
- For MPI:
  ```bash
  cd mpi/
  mpicxx -o mpi_search main.cpp
### Usage
- For OpenMP:
  ```bash
  ./ompprefixsum
- For MPI:
  ```bash
  mpiexec -np <num_processes> ./mpi_search
---
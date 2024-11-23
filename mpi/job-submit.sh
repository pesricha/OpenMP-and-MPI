#!/bin/bash
#SBATCH --job-name demo_mpi
#SBATCH --tasks-per-node 32
#SBATCH --nodelist node[4-7]
#SBATCH --cpus-per-task = 1

module load openmpi

cd $SLURM_SUBMIT_DIR
make
echo "| Processes | Avg time (ms)|"
echo "|-----------|--------------|"
mpiexec -n 1 ./main
mpiexec -n 2 ./main
mpiexec -n 4 ./main
mpiexec -n 8 ./main
mpiexec -n 16 ./main
mpiexec -n 32 ./main
mpiexec -n 64 ./main
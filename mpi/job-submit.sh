#!/bin/bash
#SBATCH --job-name demo_mpi
#SBATCH --tasks-per-node 32
#SBATCH --nodelist node[4-7]

module load openmpi

cd $SLURM_SUBMIT_DIR
make
mpiexec -n 4 ./main
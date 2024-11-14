#!/bin/bash
#SBATCH --job-name=namanp-serial-find-elem
#SBATCH --tasks-per-node=1
#SBATCH --nodelist=node4

cd $SLURM_SUBMIT_DIR
make
./main
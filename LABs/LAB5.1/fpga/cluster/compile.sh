#!/bin/bash
#SBATCH --job-name=lab5.1
#SBATCH -N 1 # number of nodes
#SBATCH -c 2 # number of cores
#SBATCH -p nebula.q

module load intelfpga-opencl/17.1

time aoc contrast.cl -l doContrast.aoclib

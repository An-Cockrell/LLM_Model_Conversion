#!/bin/bash

#SBATCH --partition=dggpu
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --gres=gpu:2
#SBATCH --mem-per-cpu=40G
#SBATCH --time=48:00:00
#SBATCH --job-name=generate_embeddings_programming_guide --output=%x.out

source ~/.bashrc
conda activate LLM

# change to the directory where you submitted this script
cd ${SLURM_SUBMIT_DIR}
# Executable section: echoing some Slurm data
echo "Starting sbatch script myscript.sh at:`date`"
echo "Running host:    ${SLURMD_NODENAME}"
echo "Assigned nodes:  ${SLURM_JOB_NODELIST}"
echo "Job ID:          ${SLURM_JOBID}"
echo "GPU(s):          ${CUDA_VISIBLE_DEVICES}"


python3 -u get_programming_guide_embeddings.py

#!/bin/bash

PROJECT_PATH="$(dirname $(dirname "$(readlink -f "${BASH_SOURCE[0]}")"))"
SLURM_ACCOUNT="INSERT_SLURM_GROUP_HERE" # get possibilities using `id`
IMAGES_DIR="INSERT_PATH_HERE" # path to `delab-images` on rabl share on DE-Lab
TIME="36:00:00"

node_config=("INTEL2" "AMD1")
declare -A nodenames
declare -A partitions
declare -A num_cpus
declare -A images
declare -A arch
declare -A additional_config

images["x86_64"]="x86_ubuntu22_04.sqsh"
images["aarch64"]="arm_ubuntu22_04.sqsh"


partitions["INTEL1"]="sorcery"
nodenames["INTEL1"]="gx21"
num_cpus["INTEL1"]="42" # could be 80
arch["INTEL1"]="x86_64"
additional_config["INTEL1"]=""


partitions["INTEL2"]="magic"
nodenames["INTEL2"]="cx01,cx02,cx03,cx04,cx05,cx06,cx07,cx08"
num_cpus["INTEL2"]="72"
arch["INTEL2"]="x86_64"
additional_config["INTEL2"]=""

partitions["INTEL3"]="alchemy"
nodenames["INTEL3"]="nx05,nx06"
num_cpus["INTEL3"]="128"
arch["INTEL3"]="x86_64"
additional_config["INTEL3"]=""

partitions["AMD1"]="magic"
nodenames["AMD1"]="cx21,cx22,cx23,cx24,cx25,cx26,cx27,cx28"
num_cpus["AMD1"]="128"
arch["AMD1"]="x86_64"
additional_config["AMD1"]=""

partitions["AMD2"]="sorcery"
nodenames["AMD2"]="gx03,gx04,gx05,gx06"
num_cpus["AMD2"]="48"
arch["AMD2"]="x86_64"
additional_config["AMD2"]=""

partitions["ARM1"]="magic"
nodenames["ARM1"]="ca01,ca02,ca03,ca04,ca05,ca06"
num_cpus["ARM1"]="48"
arch["ARM1"]="aarch64"
additional_config["ARM1"]="#SBATCH --mem=29G"

partitions["ARM2"]="sorcery"
nodenames["ARM2"]="ga01,ga02"
num_cpus["ARM2"]="72"
arch["ARM2"]="aarch64"
additional_config["ARM2"]=""
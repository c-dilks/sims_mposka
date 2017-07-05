#!/bin/bash -f

export SIMHOME=/home/dilks/sims_mposka

source ${SIMHOME}/envGeantBS.sh
source ${SIMHOME}/SetFMSEnv.sh
#source ${SIMHOME}/builds/root/bin/thisroot.sh
#testing using root in /builds
#source /builds/root/bin/thisroot.sh

export Macfile="${SIMHOME}/geant/fmsu/macros/macro"
#export ROOTSYS=${SIMHOME}/builds/root
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${ROOTSYS}/lib:${SIMHOME}/builds:${SIMHOME}/geant/fmsu/myRootClass
export MANPATH=${MANPATH}:${ROOTSYS}/man

export SIMGENENV="SIMGENENV"
export SIMGEN="${SIMHOME}/geant/fmsu/simgen"
export SIMGENLIB="SimGen.so"
export PYTHIA6="${SIMHOME}/builds/pythia6"


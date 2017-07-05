#!/bin/tcsh -f

setenv SIMHOME ${PWD}

source ${SIMHOME}/envGeantBS.tcsh
source ${SIMHOME}/SetFMSEnv

#cannot source thisroot.csh from within script with just a normal call or normal call and directory, known issue.
cd ${SIMHOME}/builds/root/bin/; source thisroot.csh 
cd ${SIMHOME}

setenv Macfile "/home/branden/simulations/geant/fmsu/macros/macro"
setenv ROOTSYS ${SIMHOME}/builds/root
setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${SIMHOME}/builds:${LD_LIBRARY_PATH}:${SIMHOME}/geant/fmsu/myRootClass
setenv MANPATH ${MANPATH}:${ROOTSYS}/man

setenv SIMGENENV "SIMGENENV"
setenv SIMGEN "${SIMHOME}/geant/fmsu/simgen"
setenv SIMGENLIB "SimGen.so"
setenv PYTHIA6 "${SIMHOME}/builds/pythia6"


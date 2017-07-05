#!/bin/tcsh -f
source ${HOME}/simulations/SetFMSEnv

#Uncomment next line if you want to use my build of root
# source ${HOME}/simulations/builds/root/bin/thisroot.csh

#Uncomment these lines if you wnat to use my build of geant
#source ${HOME}/builds/geant-install/bin/geant4.csh
#source ${HOME}/simulations/envGeantBS.tcsh

#Uncomment these lines if you want to use my pythia6 build
#setenv PYTHIA6 "${HOME}/simulations/builds/pythia6"
#setenv SIMGENENV "SIMGENENV"
#setenv SIMGEN "${HOME}/simulations/geant4/fmsu/simgen"
#setenv SIMGENLIB "SimGen.so"

#line used to name and place macrofiles generated from pythia 
setenv Macfile "${HOME}/simulations/geant4/fmsu/macros/macro"

#Adding this directory (which contains a rootmap) allows root to locate class info from startup 
setenv LD_LIBRARY_PATH  ${LD_LIBRARY_PATH}:${HOME}/simulations/geant/fmsu/myRootClass

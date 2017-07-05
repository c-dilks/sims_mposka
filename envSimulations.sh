#!/bin/bash
export Macfile="${HOME}/simulations/geant4/fmsu/macros/macro"
export ROOTSYS=${HOME}/simulations/builds/root/
export PATH=$PATH:$ROOTSYS/bin:${HOME}/simulations/builds
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib:${HOME}/simulations/builds:${HOME}/simulations/geant/fmsu/myRootClass
export MANPATH=$MANPATH:$ROOTSYS/man

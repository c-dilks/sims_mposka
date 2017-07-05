#!/bin/bash
# Generates and runs macro file for Chris (different energy depositions curves vs initial energy)

xdir=$1
ydir=$2
zdir=$3

particle="gamma"
runtimes=10
startpos="70 2.5 -10"
echo "startpos = "$startpos
echo "xdir = "$xdir
echo "ydir = "$ydir
echo "zdir = "$zdir

echo "sending $runtimes ${particle}s with momentum ($xdir,$ydir,$zdir)"

# set initial energy in {energy}
for energy in {10..80..5}
do
  echo "energy = "$energy
  # set file names for macfile and output file
  fname="_${particle}_${energy}GeV_${xdir}_${ydir}_${zdir}"

  ofile="outroot/gFile${fname}"
  ffile="outroot/fout${fname}"
  macfile="outroot/macro${fname}.mac"

  # build macrld macro
  cat vis0.mac > $macfile
  echo "/gps/position ${startpos} cm" >> $macfile

  #change back to gamma
  echo "/gps/particle ${particle}" >> $macfile
  echo "/gps/direction ${xdir} ${ydir} ${zdir}" >> $macfile
  #change energy with ${energy}
  echo "/gps/energy ${energy} GeV" >> $macfile
      
  #testing this change number of runs here
  echo "/run/beamOn ${runtimes}" >> $macfile

  echo "exit" >> $macfile
  echo "$macfile built"

  # run macro
  ${SIMHOME}/geant/bin/Linux-g++/simutesting $macfile $ofile
  touch gFile.root
  echo "simulation complete"
done

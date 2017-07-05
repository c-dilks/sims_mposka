#!/bin/bash -f
export SETFMSENV="SETFMSENV"
echo "SIMHOME = ${SIMHOME}"
#unexport FMSROOT
export FMSROOT="${SIMHOME}/analysis/root12fms"
export FMSSRC="${FMSROOT}/FpdRoot"
export FMSTXT="${FMSROOT}/fmstxt"
#export FMSGAIN="fpdgain.txt"
export FMSGAIN="FmsPtGainR16.txt"
export RunDepPath="$FMSTXT/RunDep15.root"

#export QTMAP="qtmap2009V1.txt"

#Uncomment=next=line=to=use=2012=remapping=
export QTMAP="qtmap2012V1.txt"

export QTMAP2PP="qtmap2pp.txt"
export FMSCORR="FmsCorrD70.txt"
#export FMSCORR="FmsCorrD73.txt"
export DataRoot="/GreyDisk1/Run15/"
export GetFakeSpinInfo="false"
export Keepfiles="true"
#comment=out=next=line=if=you=want=to=keep=the=filesxxxxx=directory
#unexport Keepfiles







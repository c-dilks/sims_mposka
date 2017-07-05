#!/bin/tcsh

#used to run setup variables properly when running from another computer
set dummy $HOME
setenv HOME /home/branden
source envSimulations.tcsh
setenv HOME dummy

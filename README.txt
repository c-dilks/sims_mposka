Branden Summa 01/14/16

This directory contains a condensed form of the simulations and relevent programs involved from pythia->geant->analysis

Note that I've tried to label some (though not all) of the important changes I've made with a BS in the comments, so if you are trying to search for my stuff that's a good place to start

Just in case I've included a copy of all my builds in the builds directory

Also know that I may have messed up permissions with a chmod, so let me know if this bothers you.

*****Setting Enviroment*****

UPDATE::In order to run the particle generation in pythia, you have to use my version of root, since the system one isn't built with Pythia. Additionally, I cannot get the right libries to be referenced in tcshell, despite numerous attempts, therefore the programs must currently be run in bash. From the simulations directory all one has to do is source envFinal.sh. IGNORE THE REST OF THIS SECTION

First, just in case you are using a differnt version of root than the one I compiled my G4QT class with, run RootVerMismatchFix.tcsh. This recompiles the library

NOTE::If you are running from /home/branden instead of your own accout, source envFake.tcsh instead of envSimulations.tcsh 

NOTE: This implimentation assumes that this file and the relevent directories are located at a directory called ~/simulations. If you have not done so, create that directory and move the code there. Will adjust later.

To set the enviromental variables, open tcsh and source envSimulations.tcsh. This will source envGeantBS.tcsh and SetFmsEnv automatically.


***********PYTHIA/GEANT***********

**Pythia**
The particle generation and detector simulation are both under geant/fmsu.

The pythia simulation is under the geant/fmsu/simgen. You can use the command
   root "GenerateEvents.C(x)" 
to generate x events (note that i usually add -b -q since the program creates a bunch of plots). 

The relevent cxx and h files can be found in the src directory. The most important one is in the EventLoop program, as that is where the cuts on which events and which particles within that event to keep are made. As of now it's only keeping 10 GeV or higher pi0's aimed at FMS (though no cut to make sure there's only one in an event).

The output from this program should go into the ~/simulations/geant/fmsu/macros directory. Each of these files contain the directions to be used by Geant.

**Geant**

Running shoot-particle shoots a single particle with user specified parameters.
Running shoot-multiple uses condor to run all the macro files found in the macros directory.

The GEANT main code is simutesting.cc, with it's hh files in include and cc files in src. The only one I think you might be interested in is EventAction, as it's constructor is where my tree, called "testtree" is defined.

NOTE: IGNORE THE nqtdata and Qtdata in testtree, that was the old implimentation. Note further that my class objects are stored as std::vector to enable them to be different sizes for different events when I expand gFiles later

In any case the output of geant will be sent to the ~/simulations/geant/fmsu/gFiles directory and be named gFile*.root. If you've changed LD_LIBRARY_PATH by sourcing my env file you should be able to open these files as is to take a look.


**G4QT**
My class and it's related functions are found in simulations/geant/fmsu/myRootClass/

I've included a demonstration on how to extract my class from the rootfile in this directory called ExReadG4QT.cxx



******ANALYSIS****

The goal of the ~/simulations/analysis directory is to convert the gFiles to OFiles.

the root12fms directory contains a copy of the the usual one found in the h6 directory, besides the SetFMSEnv I think the only things I have changed are AnalTools and dataSet, both located in the FpdRoot directory. Those changes can be found quickly by searching for "hsimu" or "testtree"


the analysis/myprograms directory contains some more codes I've modfified, most importantly Mkfiles.

To create an OFile, one should run Mkfiles #, where # should start with 1. I suggest using 18*, since I've been using 19* to do my own debugging. I've made some slight adjustments to the old Makefiles to have it operate properly. Besides setting some parameters normally found in MkRun, I also use this program to create fake run*.root files. The program will add up any gFiles in the gFiles directory to create the proper run.root file. and then use the myprograms/Call* methods and those found in myprograms/datadir_files. Which tree to search for has been changed in the Call* methods, as can be seen by searching hsimu within them.

Note that Mkfiles creates a directory which have copies of the program as well as runlogs. However it will fail if the directory and/or the run#.root file exist already. If you'd like you can delete those as you please. So far the most useful of which I've found so far is in the numbered subdirectory files#/#_1 where number is the the # arguement in Mkfiles.

I've included 10 gfiles in the proper directory for practice. 

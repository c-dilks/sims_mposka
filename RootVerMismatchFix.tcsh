#!/bin/tcsh -f

#BS. Depending on whether or not you are using the same version of root (5.34/23 for this directory) ROOT may complain that your ROOT and the version used in the G4QT library differ. To fix this all you need to do is remake the library using your version of root
set G4QTDIR="geant/fmsu/myRootClass" #Path to where library is located
touch $G4QTDIR/G4QT.h
make -C $G4QTDIR

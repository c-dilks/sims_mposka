#=unofficial
#export G4PREFIX=/home/branden/builds/geant4-install
export G4PREFIX=${SIMHOME}/builds/geant4-install
export COOKIE=0.0

#=official
export G4SYSTEM=Linux-g++
export G4INSTALL=$G4PREFIX/share/Geant4-9.5.1/geant4make
export G4INCLUDE=$G4PREFIX/include/Geant4
export G4LIB=$G4PREFIX/lib/Geant4-9.5.1
export G4WORKDIR=${SIMHOME}/geant
export G4LIB_BUILD_SHARED=1
export G4LIB_USE_ZLIB=1
export G4UI_USE_TCSH=1
export LD_LIBRARY_PATH=$G4PREFIX/lib:$LD_LIBRARY_PATH


#=data
export G4LEDATA=$G4PREFIX/data/G4EMLOW6.23
export G4LEVELGAMMADATA=$G4PREFIX/data/PhotonEvaporation2.2
export G4NEUTRONHPDATA=$G4PREFIX/data/G4NDL4.0
export G4RADIOACTIVEDATA=$G4PREFIX/data/RadioactiveDecay3.4
export G4ABLADATA=$G4PREFIX/data/G4ABLA3.0
export G4REALSURFACEDATA=$G4PREFIX/data/RealSurface1.0
export G4NEUTRONXSDATA=$G4PREFIX/data/G4NEUTRONXS1.1
export G4PIIDATA=$G4PREFIX/data/G4PII1.3


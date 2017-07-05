# unofficial
setenv G4PREFIX ${SIMHOME}/builds/geant4-install
setenv COOKIE 0.0

# official
setenv G4SYSTEM Linux-g++
setenv G4INSTALL $G4PREFIX/share/Geant4-9.5.1/geant4make
setenv G4INCLUDE $G4PREFIX/include/Geant4
setenv G4LIB $G4PREFIX/lib/Geant4-9.5.1
setenv G4WORKDIR ${SIMHOME}/geant
setenv G4LIB_BUILD_SHARED 1
setenv G4LIB_USE_ZLIB 1
setenv G4UI_USE_TCSH 1
setenv LD_LIBRARY_PATH $G4PREFIX/lib:$LD_LIBRARY_PATH


# data
setenv G4LEDATA $G4PREFIX/data/G4EMLOW6.23
setenv G4LEVELGAMMADATA $G4PREFIX/data/PhotonEvaporation2.2
setenv G4NEUTRONHPDATA $G4PREFIX/data/G4NDL4.0
setenv G4RADIOACTIVEDATA $G4PREFIX/data/RadioactiveDecay3.4
setenv G4ABLADATA $G4PREFIX/data/G4ABLA3.0
setenv G4REALSURFACEDATA $G4PREFIX/data/RealSurface1.0
setenv G4NEUTRONXSDATA $G4PREFIX/data/G4NEUTRONXS1.1
setenv G4PIIDATA $G4PREFIX/data/G4PII1.3


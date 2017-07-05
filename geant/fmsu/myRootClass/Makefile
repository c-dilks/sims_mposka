# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

ARCH          = linux

CXX           =
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o 

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

.PHONY: All clean



ifeq ($(ARCH),linux)
# Linux with egcs, gcc 2.9x, gcc 3.x (>= RedHat 5.2)
CXX           = g++
CXXFLAGS      = -O -Wno-deprecated -fPIC -m32 -fno-inline -Wno-write-strings 
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
INCLUDE	      = $(ROOTSYS)/include/

endif

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)
#------------------------------------------------------------------------------
All: libG4QT.so 

libG4QT.so: G4QTLinkDef.h G4QT.h G4QT.cxx
	@echo "Generating Dictionary $@..."
	@rootcint -f G4QTDict.cxx -c G4QT.h G4QTLinkDef.h
	g++ G4QT.cxx G4QTDict.cxx -shared -o libG4QT.so $(CXXFLAGS) $(GLIBS) -lMinuit

clean: 
	rm libG4QT.so

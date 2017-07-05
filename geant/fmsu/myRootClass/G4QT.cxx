#include "G4QT.h"

ClassImp(G4QT)

G4QT::G4QT()
{
}

G4QT::~G4QT()
{
}

void G4QT::FillQT(UInt_t* QT)
{
  for(UInt_t i=0; i<3700; i++)
  {
    G4QT::QTdata[i]=*(QT + i); //Not a very safe method, would like to fix in teh future
  }
}

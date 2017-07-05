#ifndef G4QT_h
#define G4QT_H 1
#include "TObject.h"


class G4QT : public TObject
{
  public:
    G4QT();
    ~G4QT();
    UInt_t n_primaries;
    UInt_t nStored;
    UInt_t event;
    UInt_t nQTdata;
    UInt_t QTdata[3700];

    void SetNPrimaries(UInt_t n) { n_primaries = n; };
    void SetEvent(UInt_t ev) { event=ev; };
    void SetNQTdata(UInt_t nqt) { nQTdata = nqt; };
    void FillQT(UInt_t *QT_);
    void SetNStored(UInt_t nstor) {nStored = nstor;}

    UInt_t GetNPrimaries(){return n_primaries;}
    UInt_t GetEvent(){return event;}
    UInt_t GetNQTdata(){return nQTdata;}
    UInt_t* GetQT(){return QTdata;}

    ClassDef(G4QT,1);

};

#endif

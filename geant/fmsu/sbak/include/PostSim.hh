#ifndef PostSim_h
#define PostSim_h 1
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TMatrix.h"
#include "TObjArray.h"
#include "TString.h"
#include "CellGeo.hh"
#include "G4FileUtilities.hh"
#include "Qt.h" // must modify LDFLAGS and CPPFLAGS to compile with FMSSRC headers
#include "FilesSet.h"

class PostSim
{
  public:
    PostSim();
    ~PostSim();
    void FillMatrices();
    void ZeroMatrices();
    void Digitise(Int_t nstb_);
    void GetHT();

    int evnum;      // event number
    int nstb;       // fms coords
    int row;        //   |
    int col;        //   |
    float gain;     // pmt gain
    float corr;     // pmt gain correction
    float en;       // deposited energy
    float tot_en;   // total energy ??
    float kin;      // kinetic energy
    float verx;     // vertex location;
    float very;     //   |
    float verz;     //   |
    float px;       // normalised momentum vector at vertex
    float py;       //   |
    float pz;       //   |
    char name[32];  // particle name
    bool isOpt;     // true if photon is optical
    bool isHit;     // true if photon hits photocathode 
    int trNum;      // track id
    float cosTh;    // angle of incidence (for photons)
    int NCer;       // number of cherenkov photons
    int ltime;      // local time
    int gtime;      // global time

    int nstb_ht;    // highest tower coordinates
    int row_ht;
    int col_ht;

    TObjArray * fadc_arr[4]; // edep/(gain*corr)
    TObjArray * edep_arr[4]; // edep
    TObjArray * hits_arr[4]; // number of hits
    TObjArray * nopt_arr[4]; // number of optical photons
    TObjArray * ncer_arr[4]; // number cherenkov photons
    TObjArray * adc_arr[4]; // adc counts (via digitisation)

    TH2F * egraph_cells_L; // energy deposition vs. hits (large cells) 
    TH2F * egraph_integ_L; // energy deposition vs. hits (large integrated)
    TH2F * egraph_cells_S; // energy deposition vs. hits (small cells)
    TH2F * egraph_integ_S; // energy deposition vs. hits (small integrated)

    Int_t nstb_tr,row_tr,col_tr,chan;
    Float_t hits,edep,fit_edep,gain_,corr_,gaincorr;
    Int_t adc;
    TTree * ttr; // main tree

    UInt_t nQTdata;
    UInt_t *nQTdataPointer;
    UInt_t QTdata[3700];
    //Int_t crate[3700];
    Qt * QT;

    FilesSet * p_files;

    TTree * hsimu;

  private:
    int arr_size;
    Int_t nnr,nnc;
    TMatrix * fadc_mat[4];
    TMatrix * edep_mat[4];
    TMatrix * hits_mat[4];
    TMatrix * nopt_mat[4];
    TMatrix * ncer_mat[4];
    TMatrix * adc_mat[4];
    Float_t ppL[5];
    Float_t ppS[5];
    Float_t hits_tot,edep_tot;
    Int_t fac;

    float max_edep,max_edep_tmp;

    Bool_t success;
    
    G4FileUtilities * fu;
    G4String FMSTXT;
    G4String FMSGAIN;
    G4String FMSCORR;
    G4String QTMAP;
    G4String QTMAP2PP;

};

#endif

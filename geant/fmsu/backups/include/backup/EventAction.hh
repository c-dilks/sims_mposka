#ifndef EventAction_h
#define EventAction_h 1
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "G4UserEventAction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "FMSlargeHit.hh"
#include "FMSsmallHit.hh"
#include "CellGeo.hh"
#include "StackingAction.hh"
#include "PostSim.hh"
#include "Qt.h"
#include "Trajectory.hh"
#include <stdio.h>
#include "/home/demaio/simulations/geant/fmsu/myRootClass/G4QT.h"
#include <vector>

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
  EventAction(FILE* fp0,TFile* p0file,TTree* geotr,Int_t vntnmbr);
  ~EventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    FILE* fp;
    TString hi;
    TH1F* ph1EL;
    TH1F* ph1ES;
    TH1F* zPhL;
    TH1F* zPhS;
    TFile* pFile;
    TTree* pgeotr;
    int evnum_;
    int rowg;
    int colg;
  Int_t eventNum;
    int nstbg;
    float gaing;
    float corrg;
    PostSim * pss;

    TTree *testtree;
/*
    TBranch *br_nsaved;
    TBranch *br_parentID;
    TBranch *br_trackID;
    TBranch *br_mass;
    TBranch *br_px;
    TBranch *br_py;
    TBranch *br_pz;
    TBranch *br_vx;
    TBranch *br_vy;
    TBranch *br_vz;
    TBranch *br_nqtdata;
    TBranch *br_QTdata;
*/
    

    Int_t nsaved_traj;
    UInt_t nhits;
    Int_t parentID[20];
    Int_t trackID[20];
    Double_t mass[20];
    Double_t energy[20];
    Double_t vx[20];
    Double_t vy[20];
    Double_t vz[20];
    Double_t px[20];
    Double_t py[20];
    Double_t pz[20];

    Trajectory *trj;
    G4QT test;
    std::vector<G4QT> myvectest;

  private:
	int max;
    G4int collectionIDL;
    G4int collectionIDS;
    G4int verboseLevel;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: EventAction2.hh,v 1.8 2006/06/29 17:47:35 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#ifndef EventAction2_h
#define EventAction2_h 1
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "G4UserEventAction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "FMSlargeHit.hh"
#include "FMSsmallHit.hh"
#include "CellGeo.hh"
#include "StackingAction.hh"

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction2 : public G4UserEventAction
{
  public:
  EventAction2(FILE* fp0,TFile* p0file,TTree* ptrel,TTree* ptres,
    TTree* geotr);
  ~EventAction2();

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
    TTree* pGtrL;
    TTree* pGtrS;
    TTree* pgeotr;
    //int evnumL_;
    //int evnumS_;
    int evnum_;
    int nstbL_;
    int nstbS_;
    int rowL_;
    int rowS_;
    int colL_;
    int colS_;
    float gainL_;
    float gainS_;
    float corrL_;
    float corrS_;
    float enL_;
    float enS_;
    float tot_enL_;
    float tot_enS_;
    float kinL_;
    float kinS_;
    float posxL_;
    float posyL_;
    float poszL_;
    float posxS_;
    float posyS_;
    float poszS_;
    float pxL_;
    float pyL_;
    float pzL_;
    float pxS_;
    float pyS_;
    float pzS_;
    char nameL_[64];
    char nameS_[64];
    int isOpticalL_;
    int isOpticalS_;
    int isHitL_;
    int isHitS_;
    int trNumL_;
    int trNumS_;
    float cosThL_;
    float cosThS_;
    int NCerL_;
    int NCerS_;
    float ltimeL_;
    float ltimeS_;
    float gtimeL_;
    float gtimeS_;
    int rowg;
    int colg;
    int nstbg;
    float gaing;
    float corrg;

  private:
    G4int collectionIDL;
    G4int collectionIDS;
    G4int verboseLevel;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    

// OLD EVENT ACTION FROM "FMS" (to compare to new one)
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
// $Id: EventAction2.cc,v 1.11 2006/06/29 17:48:05 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction2.hh"
#include "FMSlargeHit.hh"
#include "FMSsmallHit.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "Trajectory.hh"
#include "G4ios.hh"
#include "stdio.h"
#include "TTree.h"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction2::EventAction2(FILE* fp0,TFile* tf0,TTree* ptrel,TTree* ptres,
  TTree* geotr)
{
  fp=fp0;hi="hi";
  G4cout<<hi<<" from Event Action Construction"<<G4endl;
  ph1EL=0;
  ph1ES=0;
  pFile=tf0;
  zPhL=0;
  zPhS=0;
  isOpticalL_=0;
  isOpticalS_=0;
  NCerL_=0;
  NCerS_=0;
  pGtrL=ptrel;
  pGtrS=ptres;
  pgeotr=geotr;
  ltimeL_=0;
  ltimeS_=0;
  gtimeL_=0;
  gtimeS_=0;

  pGtrL->Branch("evnum",&evnum_,"evnum/I");          // event number
  pGtrL->Branch("nstb",&nstbL_,"nstb/I");             // fms nstb
  pGtrL->Branch("row",&rowL_,"row/I");                // fms row 
  pGtrL->Branch("col",&colL_,"col/I");                // fms column
  pGtrL->Branch("gain",&gainL_,"gain/F");             // pmt gain
  pGtrL->Branch("corr",&corrL_,"corr/F");             // pmt gain correction
  pGtrL->Branch("en",&enL_,"en/F");                   // deposited energy
  pGtrL->Branch("tot_en",&tot_enL_,"tot_en/F");       // total energy
  pGtrL->Branch("kin",&kinL_,"kin/F");                // kinetic energy
  pGtrL->Branch("posx",&posxL_,"posx/F");             // vertex location
  pGtrL->Branch("posy",&posyL_,"posy/F");
  pGtrL->Branch("posz",&poszL_,"posz/F");
  pGtrL->Branch("px",&pxL_,"px/F");                   // (normalised) momentum at vertex
  pGtrL->Branch("py",&pyL_,"py/F");                   // of particle creation
  pGtrL->Branch("pz",&pzL_,"pz/F");
  pGtrL->Branch("name",&nameL_,"name/C");             // particle name
  pGtrL->Branch("isOpt",&isOpticalL_,"isOptical/I");  // true if photon is optical
  pGtrL->Branch("isHit",&isHitL_,"isHit/I");          // true if photon hits photocathode
  pGtrL->Branch("trNum",&trNumL_,"TrNum/I");          // track id
  pGtrL->Branch("cosTh",&cosThL_,"CosTh/F");          // photon angle of incidence
  pGtrL->Branch("NCer",&NCerL_,"NCer/I");             // no. cherenkov photons
  pGtrL->Branch("ltime",&ltimeL_,"ltime/F");          // local time
  pGtrL->Branch("gtime",&gtimeL_,"gtime/F");          // global time

  pGtrS->Branch("evnum",&evnum_,"evnum/I");
  pGtrS->Branch("nstb",&nstbS_,"nstb/I");
  pGtrS->Branch("row",&rowS_,"row/I");
  pGtrS->Branch("col",&colS_,"col/I");
  pGtrS->Branch("gain",&gainS_,"gain/F");
  pGtrS->Branch("corr",&corrS_,"corr/F");
  pGtrS->Branch("en",&enS_,"en/F");
  pGtrS->Branch("tot_en",&tot_enS_,"tot_en/F");
  pGtrS->Branch("kin",&kinS_,"kin/F");
  pGtrS->Branch("posx",&posxS_,"posx/F");
  pGtrS->Branch("posy",&posyS_,"posy/F");
  pGtrS->Branch("posz",&poszS_,"posz/F");
  pGtrS->Branch("px",&pxS_,"px/F");
  pGtrS->Branch("py",&pyS_,"py/F");
  pGtrS->Branch("pz",&pzS_,"pz/F");
  pGtrS->Branch("name",&nameS_,"name/C");
  pGtrS->Branch("isOpt",&isOpticalS_,"isOptical/I");
  pGtrS->Branch("isHit",&isHitS_,"isHit/I");
  pGtrS->Branch("trNum",&trNumS_,"TrNum/I");
  pGtrS->Branch("cosTh",&cosThS_,"CosTh/F");
  pGtrS->Branch("NCer",&NCerS_,"NCer/I");
  pGtrS->Branch("ltime",&ltimeS_,"ltime/F");
  pGtrS->Branch("gtime",&gtimeS_,"gtime/F");

  pgeotr->Branch("row",&rowg,"row/I");
  pgeotr->Branch("col",&colg,"col/I");
  pgeotr->Branch("nstb",&nstbg,"nstb/I");
  pgeotr->Branch("gain",&gaing,"gain/F");
  pgeotr->Branch("corr",&corrg,"corr/F");

  /*
  G4String colName;
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  collectionIDL = SDman->GetCollectionID(colName="FMSlargeColl");
  collectionIDS = SDman->GetCollectionID(colName="FMSsmallColl");
  printf("[+] collectionIDL @ %p: %d\n",(void*) &collectionIDL,collectionIDL);
  printf("[+] collectionIDS @ %p: %d\n",(void*) &collectionIDS,collectionIDS);
  */

  //evnumL_=-1; // these are incremented at beginning
  //evnumS_=-1; //   of EndOfEventAction method (before gtre* filled)
  evnum_=-1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction2::~EventAction2()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction2::BeginOfEventAction(const G4Event*)
{
  fprintf(fp,"begin event \n");
  /*
  static int calledFirst = 1;
  if(calledFirst)
  {
    printf("<<<>>> EventAction2::BeginOfEventAction called\n");
    calledFirst = 0;
  };
  */
  printf("pGtrL @ %p\n",(void*) pGtrL);
  printf("pGtrS @ %p\n",(void*) pGtrS);
  G4cout << ">>> begin event "<< G4endl;
  if(ph1EL)delete ph1EL;
  if(ph1ES)delete ph1ES;
  ph1EL=new TH1F("ph1EL","ph1EL",100,0,10);
  ph1ES=new TH1F("ph1ES","ph1ES",100,0,10);
  if(zPhL)delete zPhL;
  if(zPhS)delete zPhS;
  zPhL=new TH1F("zPhL","zPhL",30,-25,25);
  zPhS=new TH1F("zPhS","zPhS",30,-25,25);
  // break
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction2::EndOfEventAction(const G4Event* evt)
{
  //evnumL_++;
  //evnumS_++;
  evnum_++;
  /*
  static int calledFirst = 1;
  if(calledFirst)
  {
    printf("<<<>>> EventAction2::EndOfEventAction called\n");
    calledFirst = 0;
  };
  */
  pFile->cd();
  G4int event_id = evt->GetEventID();

  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing
  //
  if (event_id < 100 || event_id%100 == 0) {
    G4cout << ">>> Event " << evt->GetEventID() << G4endl;
    G4cout << "    " << n_trajectories 
      << " trajectories stored in this event." << G4endl;
  }

  G4String colName;
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  collectionIDL = SDman->GetCollectionID(colName="FMSlargeColl");
  collectionIDS = SDman->GetCollectionID(colName="FMSsmallColl");
  printf("[+] collectionIDL @ %p: %d\n",(void*) &collectionIDL,collectionIDL);
  printf("[+] collectionIDS @ %p: %d\n",(void*) &collectionIDS,collectionIDS);

  G4HCofThisEvent* HCofEvent = evt->GetHCofThisEvent();
  FMSlargeHitsCollection* myCollectionL = 0;
  FMSsmallHitsCollection* myCollectionS = 0;
  if(HCofEvent)
  {
    //printf("<<<>>> Getting Collections...\n");
    myCollectionL = (FMSlargeHitsCollection*)(HCofEvent->GetHC(collectionIDL));
    myCollectionS = (FMSsmallHitsCollection*)(HCofEvent->GetHC(collectionIDS));
  }
  G4cout<<"Try to Print Hits"<<G4endl;
  printf("[+] myCollectionL @ %p\n",(void*) myCollectionL);
  printf("[+] myCollectionS @ %p\n",(void*) myCollectionS);
  G4cout<<"name (large) = "<< myCollectionL->GetName()<<G4endl;
  G4cout<<"name (small) = "<< myCollectionS->GetName()<<G4endl;

  G4double FpdE[1000];
  G4int nCer[1000];
  G4double Esum,nCerenkov;
  G4int nL,nS;

  /* large cell action */
  if(myCollectionL)
  {
    nL=myCollectionL->entries();
    G4cout<<"Try to Print " << nL <<" Large Cell Hits"<<G4endl;
    for(G4int jj=0;jj<1000;jj++){FpdE[jj]=0;nCer[jj]=0;};
    Esum=0; 
    nCerenkov=0;
    for(G4int jj=0;jj<nL;jj++)
    {
      isOpticalL_=0;
      if((*myCollectionL)[jj]->IsOptical())isOpticalL_=1;     
      isHitL_=0;
      if((*myCollectionL)[jj]->IsHit())isHitL_=1;

      int jc=(*myCollectionL)[jj]->GetCellNb();
      nstbL_=cellgeo->GetNstb(jc,1);
      rowL_=cellgeo->GetRow(jc,1);
      colL_=cellgeo->GetCol(jc,1);
      gainL_=cellgeo->GetGain(nstbL_,rowL_,colL_);
      corrL_=cellgeo->GetCorr(nstbL_,rowL_,colL_);
      enL_=(*myCollectionL)[jj]->GetEdep()/GeV;
      tot_enL_=(*myCollectionL)[jj]->GetTdep()/GeV;
      kinL_=(*myCollectionL)[jj]->GetKinE()/GeV;
      if(isOpticalL_==0) FpdE[jc]+=enL_;
      ph1EL->Fill((*myCollectionL)[jj]->GetEdep()/eV);
      if(isOpticalL_==1) nCer[jc]+=1;
      Esum+=(*myCollectionL)[jj]->GetEdep()/GeV;
      if(isOpticalL_==1)nCerenkov+=1;
      posxL_=(*myCollectionL)[jj]->GetPos().x()/cm;
      posyL_=(*myCollectionL)[jj]->GetPos().y()/cm;
      poszL_=(*myCollectionL)[jj]->GetPos().z()/cm;
      pxL_=(*myCollectionL)[jj]->GetMom().x();
      pyL_=(*myCollectionL)[jj]->GetMom().y();
      pzL_=(*myCollectionL)[jj]->GetMom().z();
      sprintf(nameL_,"%s",(*myCollectionL)[jj]->GetName().data());

      zPhL->Fill( poszL_);

      trNumL_=(*myCollectionL)[jj]->GetTrackNo();
      cosThL_=(*myCollectionL)[jj]->GetCosTh();
      NCerL_=(*myCollectionL)[jj]->GetnCerenkov();
      gtimeL_=(*myCollectionL)[jj]->GetGlobalTime()/ns;
      ltimeL_=(*myCollectionL)[jj]->GetLocalTime()/ns;

      if(jj<10)
      {
        printf("evnum=%d\n",evnum_);
        printf("nstb=%d\n",nstbL_);
        printf("row=%d\n",rowL_);
        printf("col=%d\n",colL_);
        printf("gain=%f\n",gainL_);
        printf("en=%f\n",enL_);
      };
      
      pGtrL->Fill();

    };
    for(G4int j1=0;j1<7;j1++)
    {
      for(G4int j2=0;j2<7;j2++)
      {
        G4int  jj=j1*7+j2;
        G4double BremWtE=1.*nCer[jj]/(nCerenkov+.01);
        G4cout<< jj<<" ->"<<BremWtE<<"(d="<<FpdE[jj]<<" c="<<nCer[jj]/913.<<") ";
        //	 fprintf(fp,"%f,",FpdE[jj]);
        fprintf(fp,"%f,",BremWtE);
      };
      G4cout<<G4endl;
    };
    G4cout<< "Esum=->"<<Esum<<" Ncerenkov="<<nCerenkov<<"("<<1.*nCerenkov/Esum<<" photons/GeV)"<<G4endl;
    fprintf(fp,"\nEsum=%f \n",Esum);
    if(evnum_==1)
    {
      ph1EL->Write();
      zPhL->Write();
      pGtrL->Write();
    };
    if(Esum>60) printf("unusually high edep (large): evnum=%d edep=%f\n",evnum_,Esum);
  };

  /* small cell action */
  if(myCollectionS)
  {
    nS=myCollectionS->entries();
    G4cout<<"Try to Print " << nS <<" Small Cell Hits"<<G4endl;
    for(G4int jj=0;jj<1000;jj++){FpdE[jj]=0;nCer[jj]=0;};
    Esum=0; 
    nCerenkov=0;
    for(G4int jj=0;jj<nS;jj++)
    {
      isOpticalS_=0;
      if((*myCollectionS)[jj]->IsOptical())isOpticalS_=1;     
      int jc=(*myCollectionS)[jj]->GetCellNb();
      isHitS_=0;
      if((*myCollectionS)[jj]->IsHit())isHitS_=1;

      nstbS_=cellgeo->GetNstb(jc,0);
      rowS_=cellgeo->GetRow(jc,0);
      colS_=cellgeo->GetCol(jc,0);
      gainS_=cellgeo->GetGain(nstbS_,rowS_,colS_);
      corrS_=cellgeo->GetCorr(nstbS_,rowS_,colS_);
      enS_=(*myCollectionS)[jj]->GetEdep()/GeV;
      tot_enS_=(*myCollectionS)[jj]->GetTdep()/GeV;
      kinS_=(*myCollectionS)[jj]->GetKinE()/GeV;
      if(isOpticalS_==0) FpdE[jc]+=enS_;
      ph1ES->Fill((*myCollectionS)[jj]->GetEdep()/eV);
      if(isOpticalS_==1) nCer[jc]+=1;
      Esum+=(*myCollectionS)[jj]->GetEdep()/GeV;
      if(isOpticalS_==1)nCerenkov+=1;
      posxS_=(*myCollectionS)[jj]->GetPos().x()/cm;
      posyS_=(*myCollectionS)[jj]->GetPos().y()/cm;
      poszS_=(*myCollectionS)[jj]->GetPos().z()/cm;
      pxS_=(*myCollectionS)[jj]->GetMom().x();
      pyS_=(*myCollectionS)[jj]->GetMom().y();
      pzS_=(*myCollectionS)[jj]->GetMom().z();
      sprintf(nameS_,"%s",(*myCollectionS)[jj]->GetName().data());
      zPhS->Fill( poszS_);

      trNumS_=(*myCollectionS)[jj]->GetTrackNo();
      cosThS_=(*myCollectionS)[jj]->GetCosTh();
      NCerS_=(*myCollectionS)[jj]->GetnCerenkov();
      gtimeS_=(*myCollectionS)[jj]->GetGlobalTime()/ns;
      ltimeS_=(*myCollectionS)[jj]->GetLocalTime()/ns;
      pGtrS->Fill();

    };
    for(G4int j1=0;j1<7;j1++)
    {
      for(G4int j2=0;j2<7;j2++)
      {
        G4int  jj=j1*7+j2;
        G4double BremWtE=1.*nCer[jj]/(nCerenkov+.01);
        G4cout<< jj<<" ->"<<BremWtE<<"(d="<<FpdE[jj]<<" c="<<nCer[jj]/913.<<") ";
        //	 fprintf(fp,"%f,",FpdE[jj]);
        fprintf(fp,"%f,",BremWtE);
      };
      G4cout<<G4endl;
    };
    G4cout<< "Esum=->"<<Esum<<" Ncerenkov="<<nCerenkov<<"("<<1.*nCerenkov/Esum<<" photons/GeV)"<<G4endl;
    fprintf(fp,"\nEsum=%f \n",Esum);
    if(evnum_==1)
    {
      ph1ES->Write();
      zPhS->Write();
      pGtrS->Write();
    };
    if(Esum>60) printf("unusually high edep (small): evnum=%d edep=%f\n",evnum_,Esum);
  };

  G4cout << "pGtrL entries=" << pGtrL->GetEntries() << G4endl;
  G4cout << "pGtrS entries=" << pGtrS->GetEntries() << G4endl;

  G4int lim=476;
  G4int det=0;

  while(1)
  {
    for(G4int i=0; i<lim; i++)
    {
      nstbg = cellgeo->GetNstb(i,det);
      rowg = cellgeo->GetRow(i,det);
      colg = cellgeo->GetCol(i,det);
      gaing = cellgeo->GetGain(nstbg,rowg,colg);
      corrg = cellgeo->GetCorr(nstbg,rowg,colg);
      pgeotr->Fill();
    };
    if(lim==476) { lim=788; det=1; }
    else if(lim==788) break;
    else break;
  };

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

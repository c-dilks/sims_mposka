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
// $Id: SteppingAction.cc,v 1.9 2006/06/29 17:48:18 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4ProcessManager.hh"
#include "StepTree.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{ 
  /* TrackId=aStep->GetTrack()->GetTrackID();
  if(TrackId==1){
  stepN=aStep->GetTrack()-> GetCurrentStepNumber();;; 
  TrackKEnergy=aStep->GetTrack()->GetKineticEnergy();
  zpos=aStep->GetPreStepPoint()->GetPosition().z();
  G4cout<<" track id="<<TrackId<<"Energy="<<TrackKEnergy<<"Zpos="<<zpos<< G4endl;
  }

/*
  G4int TrackId=aStep->GetTrack()->GetTrackID();
  const G4DynamicParticle* DParticle=aStep->GetTrack()->GetDynamicParticle();
  G4int PDGcode=DParticle->GetPDGcode(); 
  const G4VProcess* creator=aStep->GetTrack()->GetCreatorProcess();
  //G4ProcessType proctype=creator->GetProcessType();
  G4String procname="";
  if(creator)procname=creator->GetProcessName();
  G4double TrackKEnergy=aStep->GetTrack()->GetKineticEnergy();
  G4int stepN=aStep->GetTrack()-> GetCurrentStepNumber();
  
  if(stepN==1 && false) //++
  G4cout<<" track id="<<TrackId<<"step#:"<<stepN<<" Creator: "<< procname<<"  particle PDG code="<<PDGcode<<"Energy="<<TrackKEnergy<< G4endl;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // fill step tree with number of reflections
  G4StepStatus stepStat = aStep->GetPostStepPoint()->GetStepStatus();
  if(stepN==1) nref=0;
  if(stepStat==fGeomBoundary) nref++;

  G4TrackStatus trackStat = aStep->GetTrack()->GetTrackStatus();
  if(trackStat!=fAlive) {
    G4bool Optical=
      (aStep->GetTrack()->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition());
    if(Optical && 
        (
          (aStep->GetTrack()->GetVolume()->GetName()=="physiPhotoCL") ||
          (aStep->GetTrack()->GetVolume()->GetName()=="physiPhotoCS")
        )
      )
    {
      G4ThreeVector pmom = aStep->GetTrack()->GetVertexMomentumDirection();
      G4ThreeVector ppos = aStep->GetTrack()->GetVertexPosition();
//      steptree->AddEntry(ppos.x()/cm,ppos.y()/cm,ppos.z()/cm,pmom.x(),pmom.y(),pmom.z(),nref);
    };
  };

*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


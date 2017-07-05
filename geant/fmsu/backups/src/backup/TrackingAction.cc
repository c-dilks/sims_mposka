#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "Trajectory.hh"
#include "TrackInformation.hh"
#include "FMSlarge.hh"
#include "FMSlargeHit.hh"
#include <vector>
#include <iostream>

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  TrackInformation* trackInfo = (TrackInformation*)(aTrack->GetUserInformation());

  if(!trackInfo) //BS. This will cause a crash in the current implimentation because of a call to GetOrigionalTrackID in FMSlarge. Don't think this is necessary
  {
    //    G4cout<<"Pre Trackinfo returned zero "<<G4endl;
    fpTrackingManager->SetStoreTrajectory(false); 
  }
  else
  {
    if(trackInfo->GetTrackingStatus() > 0)
    {
      fpTrackingManager->SetStoreTrajectory(true);
      fpTrackingManager->SetTrajectory(new Trajectory(aTrack));
      trackInfo->SetSourceTrackInformation(aTrack);
      //      trackInfo->Print(); //Only prints for certain particles, see TrackInformation.cc for list
    }
    else
    { 
//      G4cout<<"Pre Tracking (false) "<<G4endl;
      fpTrackingManager->SetStoreTrajectory(false); }
  };

}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    TrackInformation* info = (TrackInformation*)(aTrack->GetUserInformation());
    if (info->GetTrackingStatus()==1 && !(aTrack->GetParticleDefinition()->GetParticleName().compareTo("gamma") ) && aTrack->GetParentID() != 0 ) { info->SetPrimaryPhotonID( aTrack->GetTrackID() ); }//BS, adding to help with making QT for photons

    size_t nSeco = secondaries->size(); 
    for(size_t i=0;i<nSeco;i++)
    { 
      TrackInformation* infoNew = new TrackInformation(info);

      ////////Turn secondary tracking on only if daughter of primary uncharged meson 
      if(nSeco>0 && !( aTrack->GetParticleDefinition()->GetParticleType().compareTo("meson") ) && aTrack->GetParentID()==0 && aTrack->GetParticleDefinition()->GetPDGCharge()==0)
      {
	infoNew->SetTrackingStatus(1);
      }
      else
      {
	infoNew->SetTrackingStatus(0);
      }
      //////////////////////////////////

      if (info->GetTrackingStatus() == 1) {
	(info->particle_ances).push_back(aTrack->GetTrackID());

      }
      (*secondaries)[i]->SetUserInformation(infoNew);
    }
  }
}



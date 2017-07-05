#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "Trajectory.hh"
#include "TrackInformation.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  /*
  static int calledFirst = 1;
  if(calledFirst)
  {
    printf("<<<>>> TrackingAction::PreUserTrackingAction called\n");
    calledFirst = 0;
  };
  */
  //if(aTrack)G4cout<<"aTrack defined "<<G4endl;
  // Create trajectory only for track in tracking region
  TrackInformation* trackInfo = (TrackInformation*)(aTrack->GetUserInformation());

  if(!trackInfo)
  {
    G4cout<<"Pre Trackinfo returned zero "<<G4endl;
    fpTrackingManager->SetStoreTrajectory(false); 
  }
  else
  {
    if(trackInfo->GetTrackingStatus() > 0)
    {
      fpTrackingManager->SetStoreTrajectory(true);
      fpTrackingManager->SetTrajectory(new Trajectory(aTrack));
      trackInfo->SetSourceTrackInformation(aTrack);
      //trackInfo->Print(); //++
    }
    else
    { 
      G4cout<<"Pre Tracking (false) "<<G4endl;

      fpTrackingManager->SetStoreTrajectory(false); }
  };

}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  /*
  static int calledFirst = 1;
  if(calledFirst)
  {
    printf("<<<>>> TrackingAction::PostUserTrackingAction called\n");
    calledFirst = 0;
  };
  */
  //  G4cout<<"PostUserTracking "<<G4endl; 
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    TrackInformation* info = (TrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size(); 
    //G4cout<<"Secondaries->size()= "<<nSeco<<G4endl;  //++
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        TrackInformation* infoNew = new TrackInformation(info);
        (*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
}



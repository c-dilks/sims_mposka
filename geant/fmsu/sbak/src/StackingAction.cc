
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "StackingAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "CellGeo.hh"
#include "SteppingAction.hh"


#include "TrackInformation.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction * stackaction;

  StackingAction::StackingAction()
: gammaCounter(0),otherCounter(0),statCounter(0),stage(0),cnt(0)
{
  G4double PcathEff0[32]=
  {0.025,0.030,0.038,0.046,0.054,0.062,0.070,0.081,0.092,
    0.103,0.114,0.125,0.136,0.147,0.158,0.169,0.180,0.188,
    0.193,0.200,0.195,0.190,0.178,0.166,0.145,
    0.114,0.082,0.055,0.028,0.005,0.004,0.002};

  G4double Esteps0[32]=  
  { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

  for(int n=0;n<32;n++)PcathEff[n]=PcathEff0[n];
  for(int n=0;n<32;n++)Esteps[n]=Esteps0[n];

  coordtr = new TTree();
  coordtr->Branch("name",&name_,"name/C"); // particle name
  coordtr->Branch("volume",&volume_,"volume/C"); // vertex volume
  //coordtr->Branch("material",&material_,"material/C"); // vertex volume material
  coordtr->Branch("evnum",&evnum_,"evnum/I"); // event number
  coordtr->Branch("generation",&generation_,"generation/I"); // parent id #
  coordtr->Branch("track_id",&track_id_,"track_id/I"); // particle id #
  coordtr->Branch("energy",&energy_,"energy/F"); // total energy
  coordtr->Branch("kinetic_energy",&kinetic_energy_,"kinetic_energy/F"); // KE
  coordtr->Branch("mapx",&mapx_,"mapx/F"); // vertex postion -> 2d map
  coordtr->Branch("mapy",&mapy_,"mapy/F");
  coordtr->Branch("px",&px_,"px/F"); // vertex mometum vector
  coordtr->Branch("py",&py_,"py/F");
  coordtr->Branch("pz",&pz_,"pz/F");
  coordtr->Branch("posx",&posx_,"posx/F"); // vertex position
  coordtr->Branch("posy",&posy_,"posy/F");
  coordtr->Branch("posz",&posz_,"posz/F");

  evnum_=-1; // incremented at ProcessNewEvent

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
  //printf("optical photon definition: %d",aTrack->GetDefinition());
  //printf(" real definition: %d\n",G4OpticalPhoton::OpticalPhotonDefinition());
  static int calledFirst = 1;
  if(calledFirst)
  {
    printf("<<<>>> StackingAction::ClassifyNewTrack called\n");
    calledFirst = 0;
  };
  if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  { // particle is optical photon
    // print optical photon volume info
    /*
    G4int jjj = aTrack->GetVolume()->GetCopyNo();
    G4cout << "stacking: (" << cellgeo->GetNstb(jjj,1) << "," << cellgeo->GetRow(jjj,1) << "," << cellgeo->GetCol(jjj,1) << ")" << G4endl;
    */


    //dont care about optical photons from front counter
    statCounter++;
    //if(!(statCounter%4==1)) return fKill; // kill every other photon to reduce stats
    // printf("<><><> statCounter = %d <><><>  gammaCounter = %d\n",statCounter,gammaCounter);

    if(aTrack->GetVolume()->GetName()=="FrontCell") {printf("|||1\n"); return fKill;}
    if(aTrack->GetParentID()>0)
    { // particle is secondary
      gammaCounter++;
      G4int tnum =aTrack->GetTrackID();
      cnt++;
      //	  if(fabs(aTrack->GetTotalEnergy()-.000003)>.000000001)return fKill;
      G4double ene=aTrack->GetTotalEnergy();
      //	  G4cout<< "ene_orig="<<" ";

      G4int ebin=0;
      G4int jbin=0;
      while(ebin==0 && jbin<32){if(ene<Esteps[jbin]){ebin=jbin;};jbin++;};

      G4double rnum=G4UniformRand();
      G4double rnum2=G4UniformRand();
      if(fabs(aTrack->GetMomentum().cosTheta())>2){printf("|||2\n"); return fKill;}//++
      //if(fabs(aTrack->GetMomentum().cosTheta()-cos(55*3.1415/180))>0.01) return fKill;

      if(rnum2>2.)
      {
        printf("|||3\n"); return fKill;
      }
      else
      {
        /*
           <<" "<<rnum<<" Esteps[jbin]="<<Esteps[jbin]<<
           " z="<<aTrack->GetPosition().z()<< " "<<
           aTrack->GetDefinition()->GetParticleName()<<G4endl;
           */
      };
      G4bool cut_cathode_eff=true;
      if(cut_cathode_eff)
      {
        //	      G4cout<< "ene="<<ene<<" ebin,jbin,rnum"<<ebin<<" "<< jbin<< "P="<<PcathEff[ebin]<<G4endl;

        if(ebin<1 || ebin>31){ return fKill;}
        if( rnum > PcathEff[ebin]){ return fKill;}
      };
    }
  }
  //if((gammaCounter%100)==0) G4cout << "gamma: " << gammaCounter << "   cnt: " << cnt << G4endl;
  else
  {
    otherCounter++;
    /*
       G4cout<<"Stacking:  "<< aTrack->GetDefinition()->GetParticleName()<<" accum optical photons="<<gammaCounter<<" KE="<<aTrack->GetKineticEnergy()<< 
       " z="<<aTrack->GetPosition().z()<< " "<<
       "#non-optical track="<<otherCounter<<G4endl;
       */    
  };
  //  G4cout << "Number of optical photons : "
  //	 << gammaCounter << G4endl; ( 0.0 0.50 1.2  1.5  1.9 ) 
  if(stage==0)
  {
    static G4int IDbound = 1;
    G4int  generation = aTrack->GetParentID();
    if(aTrack->GetTrackID()==1) // Primary particle
    {
      IDbound = 1; // reset IDbound
      generation = 0;
      TrackInformation* trackInfo = 
        new TrackInformation(aTrack);
      trackInfo->SetTrackingStatus(1);
      G4Track* theTrack = (G4Track*)aTrack;
      theTrack->SetUserInformation(trackInfo);
    }
    if(generation<=IDbound) 
    {
      G4ThreeVector decayMomentum = aTrack->GetMomentum();
      G4ThreeVector decayPosition = aTrack->GetPosition();
      G4double decayEnergy = aTrack->GetTotalEnergy();
      G4double kinEnergy = aTrack->GetKineticEnergy();
      G4double posx = decayPosition.getX()/cm;
      G4double posy = decayPosition.getY()/cm;
      G4double posz = decayPosition.getZ()/cm;
      G4double px = decayMomentum.getX();
      G4double py = decayMomentum.getY();
      G4double pz = decayMomentum.getZ();
      G4String decayName = aTrack->GetParticleDefinition()->GetParticleName();
      G4String decayType = aTrack->GetParticleDefinition()->GetParticleType();
      G4String volName = (aTrack->GetVolume()==NULL)? "nil":aTrack->GetVolume()->GetName();
      //G4String matName = (aTrack->GetMaterial()==NULL)? "nil":aTrack->GetMaterial()->GetName();
      G4int track_id = aTrack->GetTrackID();
      G4double zdist = fabs(posx+cellgeo->GetZdist());
      G4double xmap = zdist*98.6*px/(pz*98.77)+posx;
      G4double ymap = zdist*98.6*py/(pz*98.77)+posy;
      px_ = (float) px;
      py_ = (float) py;
      pz_ = (float) pz;
      posx_ = (float) posx;
      posy_ = (float) posy;
      posz_ = (float) posz;

      // read second generation decay modes
      if(aTrack->GetParentID()==1 && !strcmp(decayType,"meson")) IDbound++;

      // fill tree
      strcpy(name_,decayName.data());
      strcpy(volume_,volName.data());
      //strcpy(material_,matName.data());
      generation_=(int) generation;
      energy_=(float) decayEnergy/GeV;
      kinetic_energy_= (float) kinEnergy/GeV;
      mapx_=xmap;
      mapy_=ymap;
      track_id_ = (int) track_id;

      coordtr->Fill();

      // print particle name & energy to log
      //G4cout << "[+] " << decayName << " - " << decayEnergy/GeV << " GeV" << G4endl;

      // output coordinates (projected onto FMS front face)
      /*
      FILE * outFile;
      outFile = fopen("coord.dat","a");
      char coord[50];
      sprintf(coord,"%s %d %d %f %f %f\n",decayName.data(),evnum,generation,decayEnergy/GeV,xmap,ymap);
      if(outFile!=NULL)
      {
        fputs(coord,outFile);
        fclose(outFile);
      };
      */
    };
  };

  SteppingAction * stepact;
  //printf("n=%d\n",stepact->GetNref());
  

  return fUrgent;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingAction::NewStage()
{
  G4cout << "Number of optical photons produces in this event : "
    << gammaCounter << G4endl;
  stage++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingAction::PrepareNewEvent()
{
  stage=0;
  G4cout << "PrepareNewEvent"<<  G4endl;    
  gammaCounter = 0;

  evnum_++;
  printf("----- PREPARING FOR EVENT %d -----\n",evnum_);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"
#include "FMSlargeHit.hh"
#include "FMSsmallHit.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "Trajectory.hh"
#include "G4ios.hh"
#include "stdio.h"
#include "TTree.h"
#include "TVector3.h"
#include "PostSim.hh"
#include <TROOT.h> //Sebastian
#include <vector>
#include <map>//Matt

#include "TF1.h"
#include "TMath.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(FILE* fp0,TFile* tf0, TTree* geotr, Int_t eventNum)
{

	//Matt for ShowerMax
	//TFile *ShowerMaxFile= new TFile("ShowerMax.root","RECREATE");
	bool ShowerMaxOn = true;
	if(ShowerMaxOn) G4cout<< "Shower Max is On" <<G4endl;
	//	

	pFile=tf0;
	eventNumber=eventNum;

	pss.eventnumber=eventNumber;
	trj=new Trajectory();

	evnum_=-1;
	nsaved_traj=0;
	memset(parentID,0,sizeof(parentID));
	memset(trackID,0,sizeof(trackID));
	memset(mass,0,sizeof(mass));
	memset(energy,0,sizeof(energy));
	memset(vx,0,sizeof(vx));
	memset(vy,0,sizeof(vy));
	memset(vz,0,sizeof(vz));
	memset(px,0,sizeof(px));
	memset(py,0,sizeof(py));
	memset(pz,0,sizeof(pz));
	memset(nstb,0,sizeof(nstb));
	memset(row,0,sizeof(row));
	memset(col,0,sizeof(col));

	//  EventAction::PhotonWavelengthL = new TH1D("PhotonWavelengthL", "Photon Wavelength that reach photocathode, Large", 500, 300, 800);
	// EventAction::PhotonWavelengthS = new TH1D("PhotonWavelengthS", "Photon Wavelength that reach photocathode, Small", 500, 300, 800);
	//EventAction::OrigPhotonWavelengthL = new TH1D("OrigPhotonWavelengthL", "Origional Photon Wavelength that reach photocathode, Large", 500, 300, 800);
	//EventAction::OrigPhotonWavelengthS = new TH1D("OrigPhotonWavelengthS", "Origional Photon Wavelength that reach photocathode, Small", 500, 300, 800);
	//  EventAction::EnergyDepositShapeS = new TH2D("EnergyDepositShapeS","Eta vs distance from Photocathode, Small. Energy Weighted",48,-1,46,101,3,4.4);
	//  EventAction::EnergyDepositShapeL = new TH2D("EnergyDepositShapeL","Eta vs distance from Photocathode, Large. Energy Weighted",63,-1,61,101,2.5,3.5);

//Matt
	ShowerMaxHistE = new TH1D("ShowerMaxHistE","ShowerMaxHistE",60,0,60);
	ShowerMaxHistP = new TH1D("ShowerMaxHistP","ShowerMaxHistP",60,0,60);
//


	EventAction::testtree= new TTree("testtree","testtree");

	testtree->Branch("event", &(EventAction::eventNumber), "event/I");
	testtree->Branch("nsaved_traj", &(EventAction::nsaved_traj), "nsaved_traj/I");
    //Matt
	testtree->Branch("numHitsTot", &(EventAction::numHitsTot), "numHitsTot/I");
    //
	testtree->Branch("parentID", EventAction::parentID, "parentID[nsaved_traj]/I");
	testtree->Branch("trackID", EventAction::trackID, "trackID[nsaved_traj]/I");
	testtree->Branch("mass", EventAction::mass, "mass[nsaved_traj]/F");
	testtree->Branch("energy",EventAction::energy,"energy[nsaved_traj]/F");
	testtree->Branch("vx", EventAction::vx, "vx[nsaved_traj]/F");
	testtree->Branch("vy", EventAction::vy, "vy[nsaved_traj]/F");
	testtree->Branch("vz", EventAction::vz, "vz[nsaved_traj]/F");
	testtree->Branch("px", EventAction::px, "px[nsaved_traj]/F");
	testtree->Branch("py", EventAction::py, "py[nsaved_traj]/F");
	testtree->Branch("pz", EventAction::pz, "pz[nsaved_traj]/F");
	testtree->Branch("pt",EventAction::pt, "pt[nsaved_traj]/F");
	testtree->Branch("eta",EventAction::eta, "eta[nsaved_traj]/F");
	testtree->Branch("name",&name);
	testtree->Branch("nPrimaries",&nPrimaries);
	testtree->Branch("nstb",EventAction::nstb, "nstb[nsaved_traj]/I");
	testtree->Branch("row",EventAction::row, "row[nsaved_traj]/I");
	testtree->Branch("col",EventAction::col, "col[nsaved_traj]/I");
	//Matt
	if(ShowerMaxOn)
	{

		testtree->Branch("ShowerMaxArrayE",EventAction::ShowerMaxArrayE,"ShowerMaxArrayE[numHitsTot]/D");  
		testtree->Branch("ShowerMaxArrayP",EventAction::ShowerMaxArrayP,"ShowerMaxArrayP[numHitsTot]/D");  
		testtree->Branch("ShowerMaxE",&(EventAction::ShowerMaxE),"ShowerMaxE/D");
		testtree->Branch("ShowerMaxP",&(EventAction::ShowerMaxP),"ShowerMaxP/D");
		testtree->Branch("ShowerMaxHistE","TH1D",&ShowerMaxHistE);
		testtree->Branch("ShowerMaxHistP","TH1D",&ShowerMaxHistP);
		testtree->Branch("ShowerMaxS",EventAction::ShowerMaxSmall,"ShowerMaxL[1]/I");
	}	
	//	
	testtree->Branch("myvectest",&myvectest);
	testtree->Branch("energyQT",&energyQTvec);
	testtree->Branch("EnergyModVec",&EnergyModVec);
	testtree->Branch("RadDamageVec",&RadDamageVec);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
	G4cout << ">>> begin event "<< G4endl;
	pss.dummyFirstInEvent=0;

//Matt	
	ShowerMaxHistE->Reset();
	ShowerMaxHistP->Reset();
	numhitsEL=0;
	numhitsPL=0;
	numhitsES=0;
	numhitsPS=0;
	for(int i=0; i<50000000; i++)
	{
		ShowerMaxArrayE[i]=0;
		ShowerMaxArrayP[i]=0;
	};
	for(int i=0; i<60; i++)
	{
		Hist_Array[i]=0;
	};
//
	//BS, Radiation Damage Diagonsistics
	nPhotonsNormL=0; nPhotonsNormS=0; nPhotonsRadL=0; nPhotonsRadS=0;
	zPhotonsNormL=0; zPhotonsNormS=0; zPhotonsRadL=0; zPhotonsRadS=0;
	lPhotonsNormL=0; lPhotonsNormS=0; lPhotonsRadL=0; lPhotonsRadS=0;
	zAvgNormL=0; zAvgNormS=0; zAvgRadL=0; zAvgRadS=0;
	lAvgNormL=0; lAvgNormS=0; lAvgRadL=0; lAvgRadS=0;
	EdeptotL=0; EdeptotS=0;


	//Branden. I belive there's still a memory leak here in the individual TMatrices, but this allows for multi-event runs.
	pss.MatrixArray.SetOwner();
	pss.MatrixArray.Clear();
	
/*	hitMatrixNRC->SetOwner;
	hitMatrixRC->SetOwner;
	hitMatrixC->SetOwner;
	hitMatrixNRC->Clear;
	hitMatrixRC->Clear;
	hitMatrixC->Clear;
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	//Matt for ShowerMax
	bool ShowerMaxOn = true;
	if(ShowerMaxOn) G4cout<< "Shower Max is On " <<G4endl;
	//

	Int_t dummycount=0;
	pss.nprimaries = (int) evt->GetNumberOfPrimaryVertex();
	pFile->cd();

	Double_t RadParameters[3]={0.1146, 1.73, 43.4};
	TF1 *DamageFunc = new TF1("DamageFunc","gaus");
	DamageFunc->SetParameters(RadParameters);


	//More Detailed Radiation Damage Treatment
	TFile *EnergyShapeFile = new TFile("HistFiles.root","READ");

	TH1D *Par0L=(TH1D*) EnergyShapeFile->Get("EnergyDepositShapeL_0");
	TH1D *Par1L=(TH1D*) EnergyShapeFile->Get("EnergyDepositShapeL_1");
	TH1D *Par2L=(TH1D*) EnergyShapeFile->Get("EnergyDepositShapeL_2");

	TH1D *Par0S=(TH1D*) EnergyShapeFile->Get("EnergyDepositShapeS_0");
	TH1D *Par1S=(TH1D*) EnergyShapeFile->Get("EnergyDepositShapeS_1");
	TH1D *Par2S=(TH1D*) EnergyShapeFile->Get("EnergyDepositShapeS_2");

	Double_t RatioChangeL=0.005; //Change in percentage that occurs in Large Cell Signal per day. Default 0.5% = 0.005
	Double_t RatioChangeS=0.015; //Change in percentage that occurs in Small Cell Signal. Default 1.5% = 0.015

	Double_t CleanMFPL=115.;
	Double_t CleanMFPS=23.;

	Double_t Days=56;


	//Change to adjust for wavelenths later
	//
	Int_t NormalizingBinL=Par0L->FindBin(2.9);
	Int_t NormalizingBinS=Par0S->FindBin(3.7);

	Double_t RadParametersUpdatedL[3]={(*Par0L)[NormalizingBinL],(*Par1L)[NormalizingBinL],(*Par2L)[NormalizingBinL]};
	Double_t RadParametersUpdatedS[3]={(*Par0S)[NormalizingBinS],(*Par1S)[NormalizingBinS],(*Par2S)[NormalizingBinS]};

	TF1 *DamageFuncUpdated = new TF1("DamageFuncUpdated","gaus");

	DamageFuncUpdated->SetParameters(RadParametersUpdatedS);

	Double_t NormFactorS= -log( pow(1.-RatioChangeS,Days) ) / DamageFuncUpdated->Integral(0,45.);

	DamageFuncUpdated->SetParameters(RadParametersUpdatedL);

	Double_t NormFactorL= -log( pow(1.-RatioChangeL,Days) ) / DamageFuncUpdated->Integral(0,60.);






	evnum_++;
	nsaved_traj=0;
	G4QT test;

	// get number of stored trajectories
	//
	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
	G4cout<<"n_trajectories ="<<n_trajectories<<G4endl;
	for(G4int iv=0;iv<n_trajectories;iv++)
	{
		trj=(Trajectory*)((*(evt->GetTrajectoryContainer()))[iv]);
		Int_t parentID_ = (Int_t) trj->GetParentID();
		//BS. the primary particles are added in opposite order of what you would think. Also, added a storing cut on the decay mode in the PostUserTracking Action.
		max=40;
		if(nsaved_traj >= max )
		{
			G4cout<<"MAXIMUM NUMBER OF TRAJECTORIES PER EVENT REACHED, STOPPING EVENT"<<G4endl;
			break;
		}
		//BS can add additional cuts here, though may be better to not store trajectories in the first place
		else
		{

			EventAction::parentID[nsaved_traj] = trj->GetParentID();
			EventAction::trackID[nsaved_traj] = trj->GetTrackID();
			EventAction::mass[nsaved_traj] = trj->GetMass()/GeV;
			EventAction::energy[nsaved_traj] = trj->GetEnergy()/GeV;
			EventAction::vx[nsaved_traj] = (Float_t) ( trj->GetVertexPosition() ).getX()/cm;
			EventAction::vy[nsaved_traj] = (Float_t) ( trj->GetVertexPosition() ).getY()/cm;
			EventAction::vz[nsaved_traj] = (Float_t) ( trj->GetVertexPosition() ).getZ()/cm;

			EventAction::px[nsaved_traj] = (Float_t) ( trj->GetInitialMomentum() ).getX()/GeV;
			EventAction::py[nsaved_traj] = (Float_t) ( trj->GetInitialMomentum() ).getY()/GeV;
			EventAction::pz[nsaved_traj] = (Float_t) ( trj->GetInitialMomentum() ).getZ()/GeV;
			EventAction::pt[nsaved_traj]=sqrt(pow(px[nsaved_traj],2)+pow(py[nsaved_traj],2));
			EventAction::eta[nsaved_traj]=-log(tan(.5*(acos( (pz[nsaved_traj]) /( sqrt(pow(px[nsaved_traj],2)+pow(py[nsaved_traj],2) + pow(pz[nsaved_traj],2) ) ) ) ) ));

			EventAction::nstb[nsaved_traj] =  trj->GetNstb();
			EventAction::row[nsaved_traj] =  trj->GetRow();
			EventAction::col[nsaved_traj] =  trj->GetCol();

			EventAction::nsaved_traj++;
			name.push_back( (std::string) trj->GetParticleName() );
			EventAction::nPrimaries=evt->GetNumberOfPrimaryVertex();

			//BS
		};
	};


	pss.FillMatrixArray(nsaved_traj,EventAction::trackID);

	/////////////////////////////////////////////////

	G4String colName;
	G4SDManager * SDman = G4SDManager::GetSDMpointer();
	collectionIDL = SDman->GetCollectionID(colName="FMSlargeColl");
	collectionIDS = SDman->GetCollectionID(colName="FMSsmallColl");

	G4HCofThisEvent* HCofEvent = evt->GetHCofThisEvent();
	FMSlargeHitsCollection* myCollectionL = 0;
	FMSsmallHitsCollection* myCollectionS = 0;
	if(HCofEvent)
	{
		myCollectionL = (FMSlargeHitsCollection*)(HCofEvent->GetHC(collectionIDL));
		myCollectionS = (FMSsmallHitsCollection*)(HCofEvent->GetHC(collectionIDS));
	};

	G4int nCer[1000];
	G4double Esum,nCerenkov;
	G4int dummy = nsaved_traj; 

	srand(time(NULL));



//	TFile *ShowerMaxFile = new TFile("ShowerMax.root","RECREATE");   

	/*Matt
	if(ShowerMaxOn)
	{
		G4cout<<"BUG2"<<G4endl;
		//TFile *hfileTimeL = new TFile("photoTimeL.root","RECREATE");
	}
	*/


	//Matt
	TObjArray hitMatrixENRC[4][34];
	TObjArray hitMatrixERC[34];
	TObjArray hitMatrixEC;
	TObjArray hitMatrixPNRC[4][34];
	TObjArray hitMatrixPRC[34];
	TObjArray hitMatrixPC;
	TH1D * hmE[4][34][17];
	TH1D * hmP[4][34][17];


//	TH1D * ShowerMax = new TH1D ("ShowerMax","ShowerMax",60,0,60);
//	EventAction::ShowerMax = ShowerMax;
	
//	Double_t maxEdep[];
//	Double_t maxEdep_tmp;
//	Int_t maxN[];
//	Int_t maxR[];
//	Int_t maxC[];
//	Int_t maxZ[];
//
		//TH1D *photoTimeL = new TH1D("photoTimeL","photoTime",100,0,100); 
		//photoTimeL->Fill(-1);


	for(Int_t n=0; n<4; n++)
	{
		for(Int_t r=0; r<34; r++)
		{
			for(Int_t c=0; c<17; c++)
			{
				TString nameHistE[4][34][17];
				TString nameHistP[4][34][17];
				nameHistE[n][r][c] = Form("EnergyArray %d %d %d",n+1,r+1,c+1);
				nameHistP[n][r][c] = Form("PhotonArray %d %d %d",n+1,r+1,c+1);
				hmE[n][r][c] = new TH1D(nameHistE[n][r][c].Data(),nameHistE[n][r][c].Data(),60,0,60);
				hmP[n][r][c] = new TH1D(nameHistP[n][r][c].Data(),nameHistP[n][r][c].Data(),60,0,60);

				hitMatrixEC.Add(hmE[n][r][c]);
				hitMatrixERC[r]=hitMatrixEC;
				hitMatrixENRC[n][r]=hitMatrixERC[r];
				hitMatrixPC.Add(hmP[n][r][c]);
				hitMatrixPRC[r]=hitMatrixPC;
				hitMatrixPNRC[n][r]=hitMatrixPRC[r];
			};
		};
	};

//Matt
    numHitsTot=myCollectionL->entries()+myCollectionS->entries();

	/* large cell action */
//
	if(myCollectionL)
	{
		nhits=myCollectionL->entries();

		G4cout<<"Try to Print " << nhits <<" Large Cell Hits"<<G4endl;
		Esum=0; 
		nCerenkov=0;

		for(UInt_t jj=0;jj<nhits;jj++)
		{
			pss.zdepth=30.- ( (*myCollectionL)[jj]->GetPos().getZ() /cm ); //distance to end of cell

			pss.isHit=0;
			if((*myCollectionL)[jj]->IsHit())pss.isHit=1;
			pss.isOpt=0;
			//BS
			Bool_t DidSurvive=true;
			pss.isRealHit=false;
			if((*myCollectionL)[jj]->IsOptical())
			{
				pss.isOpt=1;     

				Double_t kinE= (Double_t) (*myCollectionL)[jj]->GetKinE()/GeV;
				Double_t Wave= TMath::HC()/(TMath::Qe() *kinE); //in nm

				//	OrigPhotonWavelengthL->Fill(Wave);

				//Seems that the integrator fails, for instance a particle generated at x=9 has lower probability than one generated at 0
				if(pss.isHit)
				{

					/*Matt
					  if((*myCollectionL)[jj]->IsOptical())
					  {
					  Double_t gTimeL = (*myCollectionL)[jj]->GetGlobalTime()/ns;
					  photoTimeL->Fill(gTimeL);
					  }
					 */

					TVector3 posvec( (*myCollectionL)[jj]->GetPos().getX() /cm ,(*myCollectionL)[jj]->GetPos().getY() /cm , 700.);
					Double_t HitEta= posvec.Eta();

					Double_t CosTh= (*myCollectionL)[jj]->GetCosTh();

					Int_t CurrentBin = Par0L->FindBin(HitEta);

					RadParametersUpdatedL[0]= NormFactorL * (*Par0L)[CurrentBin];
					RadParametersUpdatedL[1]=(*Par1L)[CurrentBin];
					RadParametersUpdatedL[2]=(*Par2L)[CurrentBin];



					DamageFuncUpdated->SetParameters(RadParametersUpdatedL);

					Double_t ProbabilitySurvive=100* TMath::Exp(-1.* DamageFunc->Integral(60-pss.zdepth,60) );

					Double_t ProbabilitySurviveUpdated=0;
					if( CosTh >0)
					{
						ProbabilitySurviveUpdated=100*TMath::Exp(-1./fabs(CosTh) * DamageFuncUpdated->Integral(0,pss.zdepth) );
					//interested in sifting through the data to discover new important genetic elements.

}
					else if(CosTh<0)
					{
						ProbabilitySurviveUpdated=100*TMath::Exp(-1./fabs(CosTh) *( DamageFuncUpdated->Integral(pss.zdepth,60.) + DamageFuncUpdated->Integral(0,60.) ) );
					}
					else
					{
						ProbabilitySurviveUpdated = 0;
					}

					if( rand()%100 < ProbabilitySurviveUpdated) pss.isRealHit=true;

					nPhotonsNormL+=1;
					zPhotonsNormL+=pss.zdepth;
					lPhotonsNormL+= (*myCollectionL)[jj]->GetTotalTrackLength();
					if(pss.isRealHit)
					{
						nPhotonsRadL+=1;
						zPhotonsRadL+=pss.zdepth;
						lPhotonsRadL+= (*myCollectionL)[jj]->GetTotalTrackLength();
					}


					//	  std::cout<<"Large Cell Depth ="<<pss.zdepth<<" and extra Probability to survive ="<<ProbabilitySurviveUpdated<<std::endl;


					//	PhotonWavelengthL->Fill(Wave);
				
				}


			// Matt
			if(ShowerMaxOn && numhitsPL<nhits)
			{
				Double_t zMaxL = (*myCollectionL)[jj]->GetPos().z()/cm;
				zMaxL += 30;
				ShowerMaxArrayP[numhitsPL]=zMaxL;
				ShowerMaxHistP->Fill(zMaxL);
				numhitsPL++;
			};

            }
			//
			if(ShowerMaxOn && numhitsEL<nhits)
			{
				Double_t zMaxL = (*myCollectionL)[jj]->GetPos().z()/cm;
				zMaxL += 30;
				ShowerMaxArrayE[numhitsEL]=zMaxL;
				ShowerMaxHistE->Fill(zMaxL);
				numhitsEL++;
			};
			//

            EdeptotL+=(*myCollectionL)[jj]->GetEdep()/GeV;

			int jc=(*myCollectionL)[jj]->GetCellNb();
			pss.nstb = cellgeo->GetNstb(jc,1);
			pss.row = cellgeo->GetRow(jc,1);
			pss.col = cellgeo->GetCol(jc,1);
			pss.gain = cellgeo->GetGain(pss.nstb,pss.row,pss.col);
			pss.corr = cellgeo->GetCorr(pss.nstb,pss.row,pss.col);
			pss.en = (*myCollectionL)[jj]->GetEdep()/GeV;
			pss.tot_en = (*myCollectionL)[jj]->GetTdep()/GeV;
			pss.kin = (*myCollectionL)[jj]->GetKinE()/GeV;
			if(pss.isOpt==1) nCer[jc]+=1;
			Esum+=(*myCollectionL)[jj]->GetEdep()/GeV;
			if(pss.isOpt==1)nCerenkov+=1;
			pss.verx = (*myCollectionL)[jj]->GetPos().x()/cm;
			pss.very = (*myCollectionL)[jj]->GetPos().y()/cm;
			pss.verz = (*myCollectionL)[jj]->GetPos().z()/cm;
			pss.px = (*myCollectionL)[jj]->GetMom().x();
			pss.py = (*myCollectionL)[jj]->GetMom().y();
			pss.pz = (*myCollectionL)[jj]->GetMom().z();
			sprintf(pss.name,"%s",(*myCollectionL)[jj]->GetName().data());

			/*Matt counting
			for(int n=0; n<4; n++)
			{
				for(int r=0; r<34; r++)
				{
					for(int c=0; c<17; c++)
					{
						Double_t zEn = (*myCollectionL)[jj]->GetPos().z()/cm;
						zEn += 30;
						hm[n][r][c]->Fill(zEn,pss.en);
					}
				}
			}
			*/
						


			pss.trNum = (*myCollectionL)[jj]->GetTrackNo();
			pss.cosTh = (*myCollectionL)[jj]->GetCosTh();
			pss.NCer = (*myCollectionL)[jj]->GetnCerenkov();
			pss.gtime = (*myCollectionL)[jj]->GetGlobalTime()/ns;
			pss.ltime = (*myCollectionL)[jj]->GetLocalTime()/ns;
			pss.originalID = (*myCollectionL)[jj]->GetOriginalID();
			pss.evnum = evnum_;
			//pss.photid = (*myCollectionL)[jj]->GetPhotonID();
			//BS
			TVector3 myvec( pss.verx/100.,pss.very/100.,7.);

			//      if(! pss.isOpt && pss.isHit) EnergyDepositShapeL->Fill(pss.zdepth,myvec.Eta(),pss.en);


			float EModPar0=1.09412 * pow(10,-8);
			float EModPar1=-0.026195;
			float EModTot= (*myCollectionL)[jj]->GetEdep()/GeV * exp(EModPar0 + EModPar1* (30.- ( (*myCollectionL)[jj]->GetPos().getZ() /cm ) ) );
			if(EModTot <0){std::cout<<"EMOD TOT LESS THAN 0!!!!!!!!!  = "<<EModTot<<std::endl; EModTot=0;}

			pss.Emod=EModTot;
			// 
			for(int index=0;index<((*myCollectionL)[jj]->GetAncestors()).size();index++)
			{
				int inderx=-1;
				bool needthis=true;
				while(inderx<nsaved_traj){
					inderx++;
					if(((*myCollectionL)[jj]->GetAncestors()).at(index)==EventAction::trackID[inderx]){break;}
					else{if(inderx==nsaved_traj-1){needthis=false;}}
				}
				if(needthis){
					if(index==0){
						pss.FillMatrices(inderx+1,true);
					}
					else{
						if(((*myCollectionL)[jj]->GetAncestors()).at(index)!=1){
							pss.FillMatrices(inderx+1,false);}
					}
				}
				else{std::cout<<"\n*****ERROR: Saved TrackID MisMatch*****\n"<<((*myCollectionL)[jj]->GetAncestors()).at(index)<<" is not in trackID[]\n";}
			}
		};
		G4cout<< "Esum=->"<<Esum<<" Ncerenkov="<<nCerenkov<<"("<<1.*nCerenkov/Esum<<" photons/GeV)"<<G4endl;
	};

	//Matt
	//TH1D *photoTimeS = new TH1D("photoTimeS","photoTime",100,0,100);

	/* small cell action */

	if(myCollectionS)
	{
		nhits=myCollectionS->entries();
		G4cout<<"Try to Print " << nhits <<" Small Cell Hits"<<G4endl;
		Esum=0; 
		nCerenkov=0;
		for(UInt_t jj=0;jj<nhits;jj++)
		{
			//if( primary!=0 && primary != (*myCollectionS)[jj]->GetOriginalID() ) {continue;}
			pss.isOpt = 0;
			int jc=(*myCollectionS)[jj]->GetCellNb();
			pss.isHit = 0;
			if((*myCollectionS)[jj]->IsHit()) pss.isHit = 1;

			pss.nstb = cellgeo->GetNstb(jc,0);

			Bool_t DidSurvive=true;
			pss.isRealHit=false;
			pss.zdepth=15.5- ( (*myCollectionS)[jj]->GetPos().getZ() /cm ); //distance to end of cell
			if((*myCollectionS)[jj]->IsOptical())
			{
				pss.isOpt=1;     

				Double_t kinE = (Double_t) (*myCollectionS)[jj]->GetKinE()/GeV;
				Double_t Wave = TMath::HC()/(TMath::Qe() *kinE); //in nm

				//	OrigPhotonWavelengthS->Fill(Wave);

				if( pss.isHit)
				{

					TVector3 posvec( (*myCollectionS)[jj]->GetPos().getX() /cm ,(*myCollectionS)[jj]->GetPos().getY() /cm , 700.);

					Double_t HitEta = posvec.Eta();
					Double_t CosTh= (*myCollectionS)[jj]->GetCosTh();
					Int_t CurrentBin = Par0S->FindBin(HitEta);

					RadParametersUpdatedS[0]= NormFactorS * (*Par0S)[CurrentBin];
					RadParametersUpdatedS[1]=(*Par1S)[CurrentBin];
					RadParametersUpdatedS[2]=(*Par2S)[CurrentBin];

					DamageFuncUpdated->SetParameters(RadParametersUpdatedS);

					Double_t ProbabilitySurviveUpdated=0;
					if(CosTh>0)
					{
						ProbabilitySurviveUpdated=100*TMath::Exp(-1./fabs(CosTh)* DamageFuncUpdated->Integral(0,pss.zdepth) );
					}
					else if (CosTh<0)
					{
						ProbabilitySurviveUpdated=100*TMath::Exp(-1./fabs(CosTh)* DamageFuncUpdated->Integral(pss.zdepth,45) + DamageFuncUpdated->Integral(0,45) );
					}
					else
					{
						ProbabilitySurviveUpdated=0;
					}
					Double_t ProbabilitySurvive=100* TMath::Exp(-1./fabs(CosTh)* DamageFunc->Integral(45-pss.zdepth,45) );
					if( rand()%100 < ProbabilitySurviveUpdated) pss.isRealHit = true;

					nPhotonsNormS+=1;
					zPhotonsNormS+=pss.zdepth;
					lPhotonsNormS+= (*myCollectionS)[jj]->GetTotalTrackLength();
					if(pss.isRealHit)
					{
						nPhotonsRadS+=1;
						zPhotonsRadS+=pss.zdepth;
						lPhotonsRadS+= (*myCollectionS)[jj]->GetTotalTrackLength();
					}
					//	  std::cout<<"Small Cell Depth ="<<pss.zdepth<<" and extra Probability to survive ="<<ProbabilitySurviveUpdated<<std::endl;
				}

                //Matt
			if(ShowerMaxOn && numhitsPS<nhits)
			{
				Double_t zMaxL = (*myCollectionL)[jj]->GetPos().z()/cm;
				zMaxL += 15.5;
				ShowerMaxArrayP[numhitsPS+numhitsPL]=zMaxL;
				ShowerMaxHistP->Fill(zMaxL);
				numhitsPS++;
			};
            //

			}
                //Matt
			if(ShowerMaxOn && numhitsES<nhits)
			{
				Double_t zMaxL = (*myCollectionL)[jj]->GetPos().z()/cm;
				zMaxL += 15.5;
				ShowerMaxArrayE[numhitsES+numhitsEL]=zMaxL;
				ShowerMaxHistE->Fill(zMaxL);
				numhitsES++;
			};
            //

			EdeptotS+=(*myCollectionS)[jj]->GetEdep()/GeV;

			pss.row = cellgeo->GetRow(jc,0);
			pss.col = cellgeo->GetCol(jc,0);
			pss.gain = cellgeo->GetGain(pss.nstb,pss.row,pss.col);
			pss.corr = cellgeo->GetCorr(pss.nstb,pss.row,pss.col);
			pss.en = (*myCollectionS)[jj]->GetEdep()/GeV;
			pss.tot_en = (*myCollectionS)[jj]->GetTdep()/GeV;
			pss.kin = (*myCollectionS)[jj]->GetKinE()/GeV;
			if(pss.isOpt==1) nCer[jc]+=1;
			Esum+=(*myCollectionS)[jj]->GetEdep()/GeV;
			if(pss.isOpt==1)nCerenkov+=1;
			pss.verx = (*myCollectionS)[jj]->GetPos().x()/cm;
			pss.very = (*myCollectionS)[jj]->GetPos().y()/cm;
			pss.verz = (*myCollectionS)[jj]->GetPos().z()/cm;
			pss.px = (*myCollectionS)[jj]->GetMom().x();
			pss.py = (*myCollectionS)[jj]->GetMom().y();
			pss.pz = (*myCollectionS)[jj]->GetMom().z();
			sprintf(pss.name,"%s",(*myCollectionS)[jj]->GetName().data());

			/*Counting
			for(int n=1; n<=4; n++)
			{
				for(int r=1; r<=34; r++)
				{
					for(int c=1; c<=17; c++)
					{
						Double_t zEn = (*myCollectionS)[jj]->GetPos().z()/cm;
						zEn += 15.5;
						hm[n-1][r-1][c-1]->Fill(zEn,pss.en);
					}
				}
			}
			*/

			// Matt
			if(ShowerMaxOn)
			{
				Double_t zMaxS = (*myCollectionS)[jj]->GetPos().z()/cm;
				zMaxS += 15.5;
				ShowerMaxS.Fill(zMaxS);
			}
			// 

			pss.trNum = (*myCollectionS)[jj]->GetTrackNo();
			pss.cosTh = (*myCollectionS)[jj]->GetCosTh();
			pss.NCer = (*myCollectionS)[jj]->GetnCerenkov();
			pss.gtime = (*myCollectionS)[jj]->GetGlobalTime()/ns;
			pss.ltime = (*myCollectionS)[jj]->GetLocalTime()/ns;
			pss.originalID = (*myCollectionS)[jj]->GetOriginalID();

			pss.evnum = evnum_;

			//BS
			TVector3 myvec( pss.verx/100.,pss.very/100.,7.);

			//      if(! pss.isOpt && pss.isHit) EnergyDepositShapeS->Fill(pss.zdepth,myvec.Eta(),pss.en);

			float EModPar0S=-2.02547 * pow(10,-8);
			float EModPar1S=-0.039964;
			float EModTot= (*myCollectionS)[jj]->GetEdep()/GeV * exp(EModPar0S + EModPar1S * (22.5 - ( (*myCollectionS)[jj]->GetPos().getZ() /cm ) ) );
			pss.Emod=EModTot;

			for(int index=0;index<((*myCollectionS)[jj]->GetAncestors()).size();index++){
				int inderxx=-1;
				bool needthis2=true;
				while(inderxx<nsaved_traj){
					inderxx++;
					if(((*myCollectionS)[jj]->GetAncestors()).at(index)==EventAction::trackID[inderxx]){break;}
					else{if(inderxx==nsaved_traj-1){needthis2=false;}}
				}
				if(needthis2){
					if(index==0){
						pss.FillMatrices(inderxx+1,true);
					}
					else{
						if(((*myCollectionS)[jj]->GetAncestors()).at(index)!=1){
							pss.FillMatrices(inderxx+1,false);}
					}
				}
				else{std::cout<<"\n*****ERROR: Saved TrackID MisMatch*****\n"<<((*myCollectionS)[jj]->GetAncestors()).at(index)<<" is not in trackID[]\n";}
			}

		};
		G4cout<< "Esum=->"<<Esum<<" Ncerenkov="<<nCerenkov<<"("<<1.*nCerenkov/Esum<<" photons/GeV)"<<G4endl;
		//   ESumTot[primary]+= (Float_t) Esum;
		//   nOpticalHits[primary]+= (UInt_t) nCerenkov;
	};

// Matt
		if(ShowerMaxOn)
		{
		Double_t RMSE = ShowerMaxHistE->GetRMS();
		Double_t binMaxE = ShowerMaxHistE->GetMaximumBin();
		ShowerMaxHistE->Fit("gaus","","",binMaxE-RMSE-0.5,binMaxE+RMSE-.5);
		EventAction::ShowerMaxE = ShowerMaxHistE->GetFunction("gaus")->GetParameter(1);
		Double_t RMSP = ShowerMaxHistP->GetRMS();
		Double_t binMaxP = ShowerMaxHistP->GetMaximumBin();
		ShowerMaxHistP->Fit("gaus","","",binMaxP-RMSP-0.5,binMaxP+RMSP-.5);
		EventAction::ShowerMaxP = ShowerMaxHistP->GetFunction("gaus")->GetParameter(1);
		}
//
	/*Matt
	maxEdep_tmp = 0;
	for(int n=0; n<4; n++)
	{
		for(int r=0; r<34; r++)
		{
			for(int c=0; c<17; c++)
			{
				maxEdep_tmp = hm[n][r][c]->GetEntries();
				if(maxEdep_tmp>maxEdep)
				{
					maxEdep.pushback(
                }
			}
		}
	}
	*/

	//BS Radiation Check
	zAvgNormL = (Double_t)  zPhotonsNormL/nPhotonsNormL;
	zAvgRadL = (Double_t)  zPhotonsRadL/nPhotonsRadL;
	lAvgNormL = (Double_t)  lPhotonsNormL/nPhotonsNormL;
	lAvgRadL = (Double_t)  lPhotonsRadL/nPhotonsRadL;

	zAvgNormS = (Double_t)  zPhotonsNormS/nPhotonsNormS;
	zAvgRadS = (Double_t)  zPhotonsRadS/nPhotonsRadS;
	lAvgNormS = (Double_t)  lPhotonsNormS/nPhotonsNormS;
	lAvgRadS = (Double_t)  lPhotonsRadS/nPhotonsRadS;

	// get HT and digitise; fill ttr & hsimu; then zero matrices
	for(int k=0;k<=nsaved_traj;k++){
		pss.GetHT(k);
		printf("HT (n%d,r%d,c%d)\n",pss.nstb_ht,pss.row_ht,pss.col_ht);

		for(int n=0; n<4; n++) pss.Digitise(n,k);

		for(Int_t dumInt=0;dumInt<3700;dumInt++)
		{
			if(dumInt<pss.nQTdata){test.QTdata[dumInt]=(pss.QTdata)[dumInt];}
			else{test.QTdata[dumInt]=0;}
		}

		for(Int_t dumIntEnergy=0;dumIntEnergy<3700;dumIntEnergy++)
		{
			if(dumIntEnergy<pss.nqtEnergy){energyG4QT.QTdata[dumIntEnergy]=(pss.QtEnergy)[dumIntEnergy]; }
			else{energyG4QT.QTdata[dumIntEnergy]=0;}
		}

		for(Int_t dumIntMod=0;dumIntMod<3700;dumIntMod++)
		{
			if(dumIntMod<pss.nqtEnergyMod){EnergyModG4QT.QTdata[dumIntMod]=(pss.QtEnergyMod)[dumIntMod];}
			else{EnergyModG4QT.QTdata[dumIntMod]=0;}
		}

		for(Int_t dumIntRad=0;dumIntRad<3700;dumIntRad++)
		{
			if(dumIntRad<pss.nqtRadDamage){RadDamageG4QT.QTdata[dumIntRad]=(pss.QtRadDamage)[dumIntRad];}
			else{RadDamageG4QT.QTdata[dumIntRad]=0;}
		}

		for(Int_t dumIntGeo1=0;dumIntGeo1<3700;dumIntGeo1++)
		{
			if(dumIntGeo1<pss.nqtGeo1){CellGeo1G4QT.QTdata[dumIntGeo1]=(pss.QtGeo1)[dumIntGeo1];}
			else{CellGeo1G4QT.QTdata[dumIntGeo1]=0;}
		}

		for(Int_t dumIntGeo2=0;dumIntGeo2<3700;dumIntGeo2++)
		{
			if(dumIntGeo2<pss.nqtGeo2){CellGeo2G4QT.QTdata[dumIntGeo2]=(pss.QtGeo2)[dumIntGeo2];}
			else{CellGeo2G4QT.QTdata[dumIntGeo2]=0;}
		}

		for(Int_t dumIntGeo3=0;dumIntGeo3<3700;dumIntGeo3++)
		{
			if(dumIntGeo3<pss.nqtGeo3){CellGeo3G4QT.QTdata[dumIntGeo3]=(pss.QtGeo3)[dumIntGeo3];}
			else{CellGeo3G4QT.QTdata[dumIntGeo3]=0;}
		}

		for(Int_t dumIntGeo4=0;dumIntGeo4<3700;dumIntGeo4++)
		{
			if(dumIntGeo4<pss.nqtGeo4){CellGeo4G4QT.QTdata[dumIntGeo4]=(pss.QtGeo4)[dumIntGeo4];}
			else{CellGeo4G4QT.QTdata[dumIntGeo4]=0;}
		}

		test.SetEvent(eventNumber);
		test.SetNQTdata(pss.nQTdata);
		test.SetNPrimaries( (UInt_t) pss.nprimaries);
		test.SetNStored( (UInt_t) dummy);
		myvectest.push_back(test);

		energyG4QT.SetEvent(eventNumber);
		energyG4QT.SetNQTdata(pss.nqtEnergy);
		energyG4QT.SetNPrimaries( (UInt_t) pss.nprimaries);
		energyG4QT.SetNStored( (UInt_t) dummy);
		energyQTvec.push_back(energyG4QT);

		EnergyModG4QT.SetEvent(eventNumber);
		EnergyModG4QT.SetNQTdata(pss.nqtEnergyMod);
		EnergyModG4QT.SetNPrimaries( (UInt_t) pss.nprimaries);
		EnergyModG4QT.SetNStored( (UInt_t) dummy);
		EnergyModVec.push_back(EnergyModG4QT);

		RadDamageG4QT.SetEvent(eventNumber);
		RadDamageG4QT.SetNQTdata(pss.nqtRadDamage);
		RadDamageG4QT.SetNPrimaries( (UInt_t) pss.nprimaries);
		RadDamageG4QT.SetNStored( (UInt_t) dummy);
		RadDamageVec.push_back(RadDamageG4QT);

		CellGeo1G4QT.SetEvent(eventNumber);
		CellGeo1G4QT.SetNQTdata(pss.nqtGeo1);
		CellGeo1G4QT.SetNPrimaries( (UInt_t) pss.nprimaries);
		CellGeo1G4QT.SetNStored( (UInt_t) dummy);
		CellGeo1Vec.push_back(CellGeo1G4QT);

		CellGeo2G4QT.SetEvent(eventNumber);
		CellGeo2G4QT.SetNQTdata(pss.nqtGeo2);
		CellGeo2G4QT.SetNPrimaries( (UInt_t) pss.nprimaries);
		CellGeo2G4QT.SetNStored( (UInt_t) dummy);
		CellGeo2Vec.push_back(CellGeo2G4QT);

		CellGeo3G4QT.SetEvent(eventNumber);
		CellGeo3G4QT.SetNQTdata(pss.nqtGeo3);
		CellGeo3G4QT.SetNPrimaries( (UInt_t) pss.nprimaries);
		CellGeo3G4QT.SetNStored( (UInt_t) dummy);
		CellGeo3Vec.push_back(CellGeo3G4QT);

		CellGeo4G4QT.SetEvent(eventNumber);
		CellGeo4G4QT.SetNQTdata(pss.nqtGeo4);
		CellGeo4G4QT.SetNPrimaries( (UInt_t) pss.nprimaries);
		CellGeo4G4QT.SetNStored( (UInt_t) dummy);
		CellGeo4Vec.push_back(CellGeo4G4QT);

	}

	testtree->Fill();

	//Replace this change into main program
	pFile->cd();
} 



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

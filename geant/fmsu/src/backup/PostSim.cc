#include "PostSim.hh"
#include "Rtypes.h"
#include "Trajectory.hh"

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "G4ThreeVector.hh"
#include "G4ios.hh"     
#include "globals.hh" 
#include "G4ParticleDefinition.hh" 
#include "G4TrajectoryPoint.hh"   
#include "G4Track.hh"
#include "G4Step.hh"
#include <vector>

PostSim::PostSim()
{
  //Jeff = new TObjArray();
  Jeff.SetOwner((Bool_t) true);
  evpos = new TObjArray();
  evpos->SetOwner((Bool_t) true);

  evnum=0;
  nstb=0;
  row=0;
  col=0;
  gain=0;
  corr=0;
  en=0;
  tot_en=0;
  kin=0;
  verx=0;
  very=0;
  verz=0;
  px=0;
  py=0;
  pz=0;
  strcpy(name,"");
  isOpt=0;
  isHit=0;
  trNum=0;
  cosTh=0;
  NCer=0;
  ltime=0;
  gtime=0;
  arr_size=0;
  nprimaries=0;
  ox=0;
  oy=0;
  
  for(int n=0; n<4; n++)
  {
    fadc_arr[n] = new TObjArray();
    edep_arr[n] = new TObjArray();
    hits_arr[n] = new TObjArray();
    nopt_arr[n] = new TObjArray();
    ncer_arr[n] = new TObjArray();
    adc_arr[n] = new TObjArray();
  };

  int nbins = 100;
  egraph_cells_L = new TH2F("egraph_cells_L","edep vs hits (large cells)",
                            nbins,0,60000,nbins,0,100);
  egraph_integ_L = new TH2F("egraph_integ_L","edep vs hits (large integrated)",
                            nbins,0,60000,nbins,0,100);
  egraph_cells_S = new TH2F("egraph_cells_S","edep vs hits (small cells)",
                            nbins,0,60000,nbins,0,100);
  egraph_integ_S = new TH2F("egraph_integ_S","edep vs hits (small integrated)",
                            nbins,0,60000,nbins,0,100);

  // set branch addresses
  ttr = new TTree("ttr","ttr");
  ttr->Branch("evnum",&evnum,"evnum/I"); // event number
  ttr->Branch("nstb",&nstb_tr,"nstb/I"); // nstb
  ttr->Branch("row",&row_tr,"row/I");    // row
  ttr->Branch("col",&col_tr,"col/I");    // col
  ttr->Branch("chan",&chan,"chan/I");    // channel 
  ttr->Branch("hits",&hits,"hits/F");    // no. hits
  ttr->Branch("edep",&edep,"edep/F");    // energy deposition
  ttr->Branch("fit_edep",&fit_edep,"fit_edep/F");  // energy dep from hits vs. edep fit
  ttr->Branch("adc",&adc,"adc/I");       // adc counts
  ttr->Branch("gain",&gain_,"gain/F");   // pmt gain
  ttr->Branch("corr",&corr_,"corr/F");   // pmt gain correction

  hsimu = new TTree("hsimu","hsimu");
  hsimu->Branch("evt",&evnum,"evt/I"); // event number
  hsimu->Branch("nqtdata",&nQTdata,"nqtdata/i"); // size of qt block
  hsimu->Branch("Qtdata",QTdata,"Qtdata[nqtdata]/i"); // qt block

  // instantiate p_files and QT
  fu = new G4FileUtilities();
  FMSGAIN = fu->GetEnv("FMSGAIN");
  FMSCORR = fu->GetEnv("FMSCORR");
  QTMAP = fu->GetEnv("QTMAP");
  QTMAP2PP = fu->GetEnv("QTMAP2PP");
  FMSTXT = fu->GetEnv("FMSTXT");
  p_files = new FilesSet((TString) FMSTXT.data(),
                         (TString) "fpdped.txt",
                         (TString) FMSGAIN.data(),
                         (TString) FMSCORR.data(),
                         (TString) "fill.txt",
                         (TString) "Fake",
                         (TString) "spinpat",
                         (TString) "geom.txt",
                         (TString) QTMAP.data(),
                         (TString) QTMAP2PP.data());
  QT = new Qt(p_files);
  nQTdataPointer = &nQTdata;
}

PostSim::~PostSim() {
  //if (Jeff) delete Jeff;
  std::cout<<"\n0\n";
  if (evpos){evpos->Delete(); delete evpos;}
  std::cout<<"\n1\n";
  if (QT){QT->Delete();}
  std::cout<<"\n2\n";
  if (p_files){p_files->Delete();}
  std::cout<<"\n3\n";
}


// adds current gtree entry to appropriate matrices/objarrays
void PostSim::FillMatrices(int i, bool t)
{
  
  // fill matrices
  TMatrix ** sfadc = (TMatrix**) Jeff.At(0);
  TMatrix ** sedep = (TMatrix**) Jeff.At(1);
  TMatrix ** shits = (TMatrix**) Jeff.At(2);
  TMatrix ** snopt = (TMatrix**) Jeff.At(3);
  TMatrix ** sncer = (TMatrix**) Jeff.At(4);
  
  TMatrix ** fadc1 = (TMatrix**) Jeff.At(i*6);
  TMatrix ** sedep1 = (TMatrix**) Jeff.At(i*6+1);
  TMatrix ** hits1 = (TMatrix**) Jeff.At(i*6+2);
  TMatrix ** nopt1 = (TMatrix**) Jeff.At(i*6+3);
  TMatrix ** ncer1 = (TMatrix**) Jeff.At(i*6+4);


  TH2D * bx = (TH2D*) evpos->At(3*i-3);
  TH2D * by = (TH2D*) evpos->At(3*i-2);
  TH2D * bz = (TH2D*) evpos->At(3*i-1);
  if(t){
  (*(sfadc[nstb-1]))(row,col) += en/(gain*corr);
  (*(sedep[nstb-1]))(row,col) += en;
  (*(sncer[nstb-1]))(row,col) += NCer;
  if(isOpt)
  {
    (*(snopt[nstb-1]))(row,col) += 1;
    if(isHit)
    {
      (*(shits[nstb-1]))(row,col) += 1;
    };
  };
  }
  (*(fadc1[nstb-1]))(row,col) += en/(gain*corr);
  (*(sedep1[nstb-1]))(row,col) += en;
  (*(ncer1[nstb-1]))(row,col) += NCer;
 if(isOpt)
   {
     (*(nopt1[nstb-1]))(row,col) += 1;
     if(isHit)
       {
	 (*(hits1[nstb-1]))(row,col) += 1;
       };
   };

  if (isHit) {
   bz->Fill(verz,en);
   bx->Fill(verx-ox,en);
   by->Fill(very-oy,en);
     }
  
}

void PostSim::wicked_pickle(int m) {
  c1=new TCanvas("c1","c1",600,700);
  c1->Print(Form("Display%i.ps(",(int)(eventnumber)));
  for(int woah=0;woah<=m;woah++){
    TMatrix ** apple = (TMatrix**) Jeff.At(6*woah+1);
    c1->Clear();
    c1->Divide(2,2);
    for(int kk=0;kk<4;kk++){
      if(hh[kk]) delete hh[kk];
      TMatrix * seagull = (TMatrix*) apple[kk];
      hh[kk]=new TH2F(*seagull);
      hh[kk]->SetName(Form("Edep #%i",kk+1));
      hh[kk]->SetTitle(Form("Energy for TrackID %i in NSTB %i is %f",woah,kk+1,seagull->Sum()));
      hh[kk]->SetStats(0);
      hh[kk]->SetMaximum(100.);
      hh[kk]->SetMinimum(0.001);
      c1->cd(kk+1);
      hh[kk]->Draw("zcol");
      hh[kk]->Draw("textsame");
      if(seagull->Sum()){
	c1->GetPad(kk+1)->SetLogz();
      }
    } 
    c1->Update();
    c1->SetTitle();
    c1->Print(Form("Display%i.ps",(int)(eventnumber)));
  }
  c1->Clear();
  c1->Print(Form("Display%i.ps)",(int)(eventnumber)));
  system(Form("ps2pdf Display%i.ps",(int)(eventnumber)));

}


void PostSim::FillRandal(int m) {
  number = 6;
  for(int i=0;i<=m;i++){
    Jeff.AddLast((TObject*) new TMatrix[4]);
    Jeff.AddLast((TObject*) new TMatrix[4]);
    Jeff.AddLast((TObject*) new TMatrix[4]);
    Jeff.AddLast((TObject*) new TMatrix[4]);
    Jeff.AddLast((TObject*) new TMatrix[4]);
    Jeff.AddLast((TObject*) new TMatrix[4]);
  }
  for(int i=0;i<=m;i++){
    TMatrix ** fadc1 = (TMatrix**) Jeff.At(i*number);
    TMatrix ** sedep1 = (TMatrix**) Jeff.At(i*number+1);
    TMatrix ** hits1 = (TMatrix**) Jeff.At(i*number+2);
    TMatrix ** nopt1 = (TMatrix**) Jeff.At(i*number+3);
    TMatrix ** ncer1 = (TMatrix**) Jeff.At(i*number+4);
    TMatrix ** adc1 = (TMatrix**) Jeff.At(i*number+5);
    for(int n=0; n<4; n++)
      {
	if(n<2) { nnr=34; nnc=17; }
	else { nnr=24; nnc=12; };
	fadc1[n] = new TMatrix(nnr,nnc);
	sedep1[n] = new TMatrix(nnr,nnc);
	hits1[n] = new TMatrix(nnr,nnc);
	nopt1[n] = new TMatrix(nnr,nnc);
	ncer1[n] = new TMatrix(nnr,nnc);
	adc1[n] = new TMatrix(nnr,nnc);
      };
  }
    ZeroMatrices(m); //BS. Method in PostSim. Sets each element of the above list of matrices to 0 with a for loop.

  for(int i = 0; i < m; i++){
    evpos->AddLast((TObject*) new TH2D());
    evpos->AddLast((TObject*) new TH2D());
    evpos->AddLast((TObject*) new TH2D());
  }
  for(int i = 0; i < m; i++){
    TH2D * temp = (TH2D*) evpos->At(i*3);
    temp->SetTitle(Form("Energy vs Xpos for TrackID %i",i+1));
    temp->SetBins(100,-90,90,100,0,.01);
    TH2D * temp2 = (TH2D*) evpos->At(i*3+1);
    temp2->SetBins(100,-90,90,100,0,.01);
    temp2->SetTitle(Form("Energy vs Ypos for TrackID %i",i+1));
    TH2D * temp3 = (TH2D*) evpos->At(i*3+2);
    temp3->SetBins(100,-35,35,100,0,.01);
    temp3->SetTitle(Form("Energy vs Zpos for TrackID %i",i+1));
  }
  
}

// sets all elements of all private matrices to zero
void PostSim::ZeroMatrices(int m)
{
   int nrows,ncols;
   for(int i=0;i<=m;i++){
     TMatrix ** fadc1 = (TMatrix**) Jeff.At(i*6);
     TMatrix ** sedep1 = (TMatrix**) Jeff.At(i*6+1);
     TMatrix ** hits1 = (TMatrix**) Jeff.At(i*6+2);
     TMatrix ** nopt1 = (TMatrix**) Jeff.At(i*6+3);
     TMatrix ** ncer1 = (TMatrix**) Jeff.At(i*6+4);
     TMatrix ** adc1 = (TMatrix**) Jeff.At(i*6+5);
     for(int n=0; n<4; n++)
       {
	 nrows=34; ncols=17;
	 if(n>1) { nrows=24; ncols=12; };
	 for(int nr=0; nr<nrows; nr++)
	   {
	     for(int nc=0; nc<ncols; nc++)
	       {
		 (*(fadc1[n]))(nr,nc)=0;
		 (*(sedep1[n]))(nr,nc)=0;
		 (*(hits1[n]))(nr,nc)=0;
		 (*(nopt1[n]))(nr,nc)=0;
		 (*(ncer1[n]))(nr,nc)=0;
		 (*(adc1[n]))(nr,nc)=0;
	       };
	   };
       };
   }
} 
// note: need to call "GetHT" before calling this
// digitise (hits matrix --> adc matrix)
// fills ttr
// encodes qt block and fills hsimu
// (see drupal entry on May 13 2013 for details)
void PostSim::Digitise(Int_t nn, int k)
{
//  StackingAction *teststack = new StackingAction();
//  TTree *mytree = teststack->GetCoordTr();
//  Int_t nevents = (Int_t) mytree->GetEntries();
//  TH1F *myhist= new TH1F("myhist","myhist",10000,0,100);
//  mytree->Draw("energy>>myhist","generation == 0");
  // digitisation fit parameters
  ppL[0] = 0;
  ppL[1] = 0.00224282; //BS, replace this with just the lin slope
  ppL[2] = -4.46158e-08;
  ppL[3] = 1.30951e-12;
  ppL[4] = -1.34152e-17;
  ppS[0] = 0;
  ppS[1] = 0.00219184;
  ppS[2] = -4.57127e-08;
  ppS[3] = 1.25689e-12;
  ppS[4] = -1.21213e-17;

  TMatrix ** sfadc = (TMatrix**) Jeff.At(k*6);
  TMatrix ** sedep = (TMatrix**) Jeff.At(k*6+1);
  TMatrix ** shits = (TMatrix**) Jeff.At(k*6+2);
  TMatrix ** snopt = (TMatrix**) Jeff.At(k*6+3);
  TMatrix ** sncer = (TMatrix**) Jeff.At(k*6+4);
  TMatrix ** sadc = (TMatrix**) Jeff.At(k*6+5);

  for(Int_t r=0; r<shits[nn]->GetNrows(); r++)
  {
    for(Int_t c=0; c<shits[nn]->GetNcols(); c++)
    {
      hits = (*(shits[nn]))(r,c);
      edep = (*(sedep[nn]))(r,c);
      if(hits>0)
      {
        gain_ = cellgeo->GetGain(nn+1,r,c);
        corr_ = cellgeo->GetCorr(nn+1,r,c);
        gaincorr = gain_*corr_;
        if(nn<2)
        {
          fac=17;
          fit_edep = ppL[0]+
                     ppL[1]*hits+
                     ppL[2]*pow(hits,2)+
                     ppL[3]*pow(hits,3)+
                     ppL[4]*pow(hits,4);
        }
        else
        {
          fac=12;
          fit_edep = ppS[0]+
                     ppS[1]*hits+
                     ppS[2]*pow(hits,2)+
                     ppS[3]*pow(hits,3)+
                     ppS[4]*pow(hits,4);
        };
        adc = (Int_t)(fit_edep/gaincorr + 0.5); // round to nearest integer
        chan = c+r*fac+1;

        (*(sadc[nn]))(r,c) = adc;

        nstb_tr = nn+1;
        row_tr = r;
        col_tr = c;
        ttr->Fill();
      };
    };
  };
  if(nn<2) egraph_integ_L->Fill(hits_tot,edep_tot);
  else egraph_integ_S->Fill(hits_tot,edep_tot);

  // encode qt block for nstb with HT only
//  if(nn+1 == nstb_ht || nstb_ht==0)
  if(nn == 3)
  {
    for(int g=0; g<3700; g++) QTdata[g]=0;
    success = QT->encodeQT(sadc,nQTdataPointer,QTdata);
    nQTdata = *nQTdataPointer;
    printf("nQTdata=%u\n",(unsigned int)nQTdata);
    hsimu->Fill();
  };


}

// loops through edep matrices and sets nstb_ht,row_ht,col_ht to
// the highest tower corrdinates
void PostSim::GetHT(int i)
{;
  TMatrix ** sedep = (TMatrix**) Jeff.At(i*6+1);
  max_edep=0;
  nstb_ht=row_ht=col_ht=0;

 for(int n=0; n<4; n++)
  {
    for(int r=0; r<sedep[n]->GetNrows(); r++)
    {
      for(int c=0; c<sedep[n]->GetNcols(); c++)
      {
        max_edep_tmp = (*(sedep[n]))(r,c);
        if(max_edep_tmp > max_edep)
        {
          max_edep = max_edep_tmp;
          nstb_ht = n+1;
          row_ht = r;
          col_ht = c;
        };
      };
    };
  };
  
}

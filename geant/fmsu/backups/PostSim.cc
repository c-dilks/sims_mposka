#include "PostSim.hh"

PostSim::PostSim()
{
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
  for(int n=0; n<4; n++)
  {
    if(n<2) { nnr=34; nnc=17; }
    else { nnr=24; nnc=12; };
    fadc_mat[n] = new TMatrix(nnr,nnc);
    edep_mat[n] = new TMatrix(nnr,nnc);
    hits_mat[n] = new TMatrix(nnr,nnc);
    nopt_mat[n] = new TMatrix(nnr,nnc);
    ncer_mat[n] = new TMatrix(nnr,nnc);
    adc_mat[n] = new TMatrix(nnr,nnc);
  };
  ZeroMatrices(); //BS. Method in PostSim. Sets each element of the above list of matrices to 0 with a for loop.

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

PostSim::~PostSim()
{
  if (QT) delete QT;
  if (p_files) delete p_files;
}


// adds current gtree entry to appropriate matrices/objarrays
void PostSim::FillMatrices()
{

  // fill matrices
  (*(fadc_mat[nstb-1]))(row,col) += en/(gain*corr);
  (*(edep_mat[nstb-1]))(row,col) += en;
  (*(ncer_mat[nstb-1]))(row,col) += NCer;
  if(isOpt)
  {
    (*(nopt_mat[nstb-1]))(row,col) += 1;
    if(isHit)
    {
      (*(hits_mat[nstb-1]))(row,col) += 1;
    };
  };
}

// sets all elements of all private matrices to zero
void PostSim::ZeroMatrices()
{
  int nrows,ncols;
  for(int n=0; n<4; n++)
  {
    nrows=34; ncols=17;
    if(n>1) { nrows=24; ncols=12; };
    for(int nr=0; nr<nrows; nr++)
    {
      for(int nc=0; nc<ncols; nc++)
      {
        (*(fadc_mat[n]))(nr,nc)=0;
        (*(edep_mat[n]))(nr,nc)=0;
        (*(hits_mat[n]))(nr,nc)=0;
        (*(nopt_mat[n]))(nr,nc)=0;
        (*(ncer_mat[n]))(nr,nc)=0;
        (*(adc_mat[n]))(nr,nc)=0;
      };
    };
  };
}

// note: need to call "GetHT" before calling this
// digitise (hits matrix --> adc matrix)
// fills ttr
// encodes qt block and fills hsimu
// (see drupal entry on May 13 2013 for details)
void PostSim::Digitise(Int_t nn)
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


  for(Int_t r=0; r<hits_mat[nn]->GetNrows(); r++)
  {
    for(Int_t c=0; c<hits_mat[nn]->GetNcols(); c++)
    {
      hits = (*(hits_mat[nn]))(r,c);
      edep = (*(edep_mat[nn]))(r,c);
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

        (*(adc_mat[nn]))(r,c) = adc;

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
    for(int k=0; k<3700; k++) QTdata[k]=0;
    success = QT->encodeQT(adc_mat,nQTdataPointer,QTdata);
    nQTdata = *nQTdataPointer;
    printf("nQTdata=%u\n",(unsigned int)nQTdata);
    hsimu->Fill();
  };


}

// loops through edep matrices and sets nstb_ht,row_ht,col_ht to
// the highest tower corrdinates
void PostSim::GetHT()
{
  max_edep=0;
  nstb_ht=row_ht=col_ht=0;

  for(int n=0; n<4; n++)
  {
    for(int r=0; r<edep_mat[n]->GetNrows(); r++)
    {
      for(int c=0; c<edep_mat[n]->GetNcols(); c++)
      {
        max_edep_tmp = (*(edep_mat[n]))(r,c);
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

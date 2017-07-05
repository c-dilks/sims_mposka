void ExReadG4QT()
{
  //Example of how to read testtree to get to my class. Class h and cxx files found in geant/fmsu/myRootClass
 
//  gSystem->Load("geant/fmsu/myRootClass/libG4QT.so"); //shouldn't be necessary if you sourced everything but just in case

//These lines just get the tree into memory, done with fChain methods in dataSet

  TFile *f=TFile::Open("gFile7343.root","READ");
  TTree *t;
  f->GetObject("testtree",t);

//  t->SetMakeClass(1);
//

  //Setting the Branch Address
  TBranch *tbr=0;
  std::vector<G4QT> *pG4QTvec=0;
  G4QT myG4QT;
  t->SetBranchAddress("myvectest",&pG4QTvec,&tbr);

  //Extracting Information
  tbr->GetEntry(0);
  Int_t whichParticle=1; //Which partilce you want QT for. Right now since theres just one pi0 0=1=info from pi0 and decentdents; 2,3 for individual photons
  myG4QT=pG4QTvec->at(whichParticle);

  UInt_t Qtdata[3700];
  G4QT *pmyG4QT=0;

  std::cout<<myG4QT.GetNPrimaries()<<std::endl;

//myG4QT now has my class, can access data through member functions
//

}

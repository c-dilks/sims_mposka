// fits shower max vs. energy points to expected form

void FitShowerMax() {
  enum sm_enum {kE,kSM};
  // matt's numbers
  const Int_t N_matt = 8;
  Float_t sm_matt[2][N_matt]; 

  sm_matt[kE][0]=10; sm_matt[kSM][0]=22.74;
  sm_matt[kE][1]=20; sm_matt[kSM][1]=23.27;
  sm_matt[kE][2]=30; sm_matt[kSM][2]=24.03;
  sm_matt[kE][3]=40; sm_matt[kSM][3]=26.41;
  sm_matt[kE][4]=50; sm_matt[kSM][4]=25.41;
  sm_matt[kE][5]=60; sm_matt[kSM][5]=25.01;
  sm_matt[kE][6]=70; sm_matt[kSM][6]=27.15;
  sm_matt[kE][7]=80; sm_matt[kSM][7]=31.12;

  TGraph * sm_matt_gr = new TGraph(N_matt,sm_matt[kE],sm_matt[kSM]);
  sm_matt_gr->SetMarkerStyle(kFullCircle);

  new TCanvas();
  sm_matt_gr->Draw("AP");
  TF1 * ftn = new TF1("ftn","[0]*TMath::Log(x/[1])/TMath::Log(2)");
  ftn->SetParName(0,"X_{0}");
  ftn->SetParName(1,"E_{0}");
  ftn->SetParameter(0,3.2);
  ftn->SetParameter(1,0.1);
  sm_matt_gr->Fit("ftn","","",10,80);
    
};

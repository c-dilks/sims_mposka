const Int_t NN = 10;
const Float_t LL = 0.1;
const Float_t WW = 2;
Double_t radlength = 2.5;
const Float_t theta0 = 0;

void PlotSSmodel() {
  TF2 * fn = new TF2("fn",&FF,-25,25,-25,25,0);
  new TCanvas();
  fn->Draw("SURF2");
}


// Sz and Sx -- source coords w.r.t. photon entry point
Double_t Sz(Int_t k0, Double_t theta0) { return k0 * TMath::Cos(theta0); };
Double_t Sx(Int_t k0, Double_t theta0) { return k0 * TMath::Sin(theta0); };


// A and B -- shower shape params
Double_t AA(Int_t k0) { 
  return TMath::Power(2,k0) / ( TMath::Power(2,NN+1) - 1 );
};

Double_t BB(Int_t k0, Double_t theta0) { 
  return ( LL + NN - Sz(k0,theta0) ) / WW;
};

// X and Y -- source coords projected to detector plane
Double_t XX(Double_t x0, Int_t k0, Double_t theta0) {
  return x0 - radlength * Sx(k0,theta0);
};

Double_t YY(Double_t y0, Int_t k0, Double_t theta0) {
  return y0;
};



// F -- CDF
Double_t FF(Double_t * x, Double_t * p) {

  Double_t FFret = 0;

  Double_t x0 = x[0];
  Double_t y0 = x[1];


  for(int k=0; k<=NN; k++) {
    FFret += 1/TMath::TwoPi() * AA(k) * (
      TMath::ATan(XX(x0,k,theta0) / BB(k,theta0)) + TMath::ATan(YY(y0,k,theta0) / BB(k,theta0)) + 
      TMath::ATan( XX(x0,k,theta0) * YY(y0,k,theta0) / (
        BB(k,theta0) * TMath::Sqrt(
          BB(k,theta0)**2 + XX(x0,k,theta0)**2 + YY(y0,k,theta0)**2)
        )
      )
    );

  };
  
  return FFret;
};



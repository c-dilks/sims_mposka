// PLOTS SHOWER SHAPE MODEL

const Int_t NN = 10;

const Int_t NPARAM = 6;
enum paramEnum {kL,kW,kTheta,kHscale,kVscale,kS};

Double_t radlength = 2.5;
const Float_t DD = 3.8;

// theta=0
const Float_t LLdefault = 0.257; // 0.2
const Float_t WWdefault = 5 * radlength; // 2.31
const Float_t thetaDefault = 20 * TMath::Pi() / 180.0;
//Float_t ssDefault = 5; // +++
Float_t ssDefault = 40; // +++

//const Float_t LLdefault = 0.2;
//const Float_t WWdefault = 1.8;
//const Float_t thetaDefault = 20 * TMath::Pi() / 180.0;
//const Float_t ssDefault = 35;

//const Float_t kHscaleDefault = 1; // +++
const Float_t kHscaleDefault = -1; // +++
const Float_t kVscaleDefault = 1.12;



Double_t plotBound = 20;

void PlotSSmodel() {


  /*
  Float_t thetaDegMin = 0; 
  Float_t thetaDegMax = 30;
  const Int_t nThetas = 4; // 4 for steps of 10; 7 for steps of 5
  */
  Float_t thetaDegMin = 0; 
  Float_t thetaDegMax = 15;
  const Int_t nThetas = 4;
  Float_t thetaDegStep = (thetaDegMax-thetaDegMin) / ((Float_t)(nThetas-1));
  Float_t thetaDegCurr, thetaCurr;

  TFile * infile = new TFile("ssplots.root","READ");
  TProfile * inplot[nThetas];
  TString inplotN[nThetas];

  TString paramN[NPARAM];
  paramN[kL] = "L";
  paramN[kW] = "W";
  paramN[kTheta] = "Theta";
  paramN[kHscale] = "H";
  paramN[kVscale] = "V";
  paramN[kS] = "S";

  Int_t colors[nThetas];
  switch(nThetas) {
    case 4:
      colors[0] = kRed;
      colors[1] = kBlue;
      colors[2] = kGreen+1;
      colors[3] = kOrange+1;
      break;
    case 7:
      colors[0] = kRed;
      colors[1] = kOrange+1;
      colors[2] = kGreen+1;
      colors[3] = kCyan+1;
      colors[4] = kBlue;
      colors[5] = kViolet;
      colors[6] = kMagenta;
  };


  //TF2 * fn[nThetas];
  TF1 * fn[nThetas];
  TString fnn[nThetas];
  //TF12 * fnX[nThetas];
  TF1 * fnX[nThetas];
  TString fnXn[nThetas];

  for(Int_t t=0; t<nThetas; t++) {
    printf("t=%d\n",t);
    thetaDegCurr = t * thetaDegStep;
    thetaCurr = thetaDegCurr * TMath::Pi() / 180.0;

    inplotN[t] = Form("ssdist_th%d",(Int_t)thetaDegCurr);
    inplot[t] = (TProfile*) infile->Get(inplotN[t].Data());

    fnn[t] = Form("fn_th%d",(Int_t)thetaDegCurr);
    fnXn[t] = Form("fnX_th%d",(Int_t)thetaDegCurr);

    //fn[t] = new TF2(fnn[t].Data(),&GG,-1*plotBound,plotBound,-1*plotBound,plotBound,NPARAM);
    fn[t] = new TF1(fnn[t].Data(),&GG,-1*plotBound,plotBound,NPARAM);
    //fnX[t] = new TF12(fnXn[t].Data(),fn[t],0,"x");
    fnX[t] = fn[t];

    fnX[t]->SetLineColor(colors[t]);
    fnX[t]->SetLineWidth(4);

    fn[t]->GetXaxis()->SetTitle("X");
    fn[t]->GetYaxis()->SetTitle("Y");

    fn[t]->SetParameter(kL,LLdefault);
    fn[t]->SetParameter(kW,WWdefault);
    fn[t]->SetParLimits(kL, 0.1, 3.0);
    //fn[t]->SetParLimits(kW, 0.1, 4.0);
    fn[t]->FixParameter(kW, WWdefault);

    fn[t]->SetParameter(kTheta,thetaCurr);
    fn[t]->FixParameter(kTheta,thetaCurr);
    
    fn[t]->SetParameter(kHscale,kHscaleDefault);
    fn[t]->SetParameter(kVscale,kVscaleDefault);
    //fn[t]->SetParLimits(kHscale, 0.1, 5.0); // +++
    fn[t]->SetParLimits(kHscale, -2.0, -0.5); // +++
    fn[t]->SetParLimits(kVscale, 0.8, 1.2);
    //fn[t]->FixParameter(kHscale,kHscaleDefault);
    //fn[t]->FixParameter(kVscale,kVscaleDefault);


    fn[t]->SetParameter(kS,ssDefault);
    //fn[t]->FixParameter(kS,ssDefault);
    fn[t]->SetParLimits(kS,ssDefault-10,ssDefault+10);

    for(int pp=0; pp<NPARAM; pp++) fn[t]->SetParName(pp,paramN[pp].Data());
  };

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);

  TCanvas * c0 = new TCanvas();
  c0->SetGrid(1,1);
  for(Int_t t=0; t<nThetas; t++) {
    if(t>0) fnX[t]->Draw("SAME");
    else fnX[t]->Draw();
  };

  for(Int_t t=0; t<nThetas; t++) {
    if(inplot[t]!=NULL) {
      if(inplot[t]->GetEntries()>0) {
        new TCanvas();

        //inplot[t]->Fit(fnX[t],"","",-4,4);
        switch(t) {
          case(0): 
            inplot[t]->Fit(fnX[t],"","",-4,4);
            break;
          case(1): 
            inplot[t]->Fit(fnX[t],"","",-4,4);
            break;
          case(2): 
            inplot[t]->Fit(fnX[t],"","",-4,4);
            break;
          case(3): 
            inplot[t]->Fit(fnX[t],"","",-4,4);
            break;
          default: break;
        };


        inplot[t]->GetYaxis()->SetRangeUser(0,1);

        inplot[t]->Draw();
        fnX[t]->Draw("SAME");
      };
    };
  };
}


// Sz and Sx -- source coords w.r.t. photon entry point
Double_t Sz(Int_t k0, Double_t theta0) { return k0 * TMath::Cos(theta0); };
Double_t Sx(Int_t k0, Double_t theta0) { return k0 * TMath::Sin(theta0); };


// A and B -- shower shape params
Double_t AA(Int_t k0) { 
  return TMath::Power(2,k0) / ( TMath::Power(2,NN+1) - 1 );
};

Double_t BB(Int_t k0, Double_t * params) { 
  Double_t LL = params[kL];
  Double_t WW = params[kW];
  Double_t theta = params[kTheta];
  //return ( LL/TMath::Cos(theta) + NN - Sz(k0,theta) ) / WW;
  return radlength * ( LL + NN - Sz(k0,theta) ) / WW;
};

// X and Y -- source coords projected to detector plane
Double_t XX(Double_t x0, Int_t k0, Double_t * params) {
  Double_t theta = params[kTheta];
  Double_t WW = params[kW];
  //return (x0 - radlength * Sx(k0,theta)) / (1 + WW*TMath::Tan(theta));
  return x0 - radlength * Sx(k0,theta);
};

Double_t YY(Double_t y0, Int_t k0, Double_t * params) {
  Double_t theta = params[kTheta];
  return y0;
};



// F -- CDF
Double_t FF(Double_t x0, Double_t y0, Double_t * params) {

  Double_t FFret = 0;


  for(int k=0; k<=NN; k++) {
    FFret += 1/TMath::TwoPi() * AA(k) * (
      TMath::ATan(XX(x0,k,params) / BB(k,params)) + 
      TMath::ATan(YY(y0,k,params) / BB(k,params)) + 
      TMath::ATan( XX(x0,k,params) * YY(y0,k,params) / (
        BB(k,params) * TMath::Sqrt(
          BB(k,params)**2 + XX(x0,k,params)**2 + YY(y0,k,params)**2)
        )
      )
    );

  };
  
  return FFret;
};


// G -- tower shape
Double_t GG(Double_t * x, Double_t * params) {

  Double_t Vscale = params[kVscale];
  Double_t Hscale = params[kHscale];
  Double_t ss = params[kS];
  Double_t theta = params[kTheta];

  Double_t xxx = x[0];
  //Double_t yyy = x[1];
  Double_t yyy = 0;

  ///*
  xxx *= Hscale;
  yyy *= Hscale;


  //xxx -= ss * TMath::Tan(theta); //+++
  xxx += ss * TMath::Tan(theta); //+++
  return Vscale * (
           FF(xxx+DD/2, yyy+DD/2, params) -
           FF(xxx-DD/2, yyy+DD/2, params) -
           FF(xxx+DD/2, yyy-DD/2, params) +
           FF(xxx-DD/2, yyy-DD/2, params) 
         );
  //*/
  /*
  return Vscale * (
           FF(Hscale*(xxx+DD/2), Hscale*(yyy+DD/2), params) -
           FF(Hscale*(xxx-DD/2), Hscale*(yyy+DD/2), params) -
           FF(Hscale*(xxx+DD/2), Hscale*(yyy-DD/2), params) +
           FF(Hscale*(xxx-DD/2), Hscale*(yyy-DD/2), params) 
         );
  */
};




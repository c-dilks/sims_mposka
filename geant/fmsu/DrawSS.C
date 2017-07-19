void DrawSS(Float_t energy0 = 60,
            Int_t lsvar0 = 1,
            Int_t hvvar0 = 0,
            Float_t frac_edep_cut = 0.001
           ) {
  TFile * outfile = new TFile("ssplots.root","RECREATE");

  TString fname = "gFiles/all.root";
  TFile * infile = new TFile(fname.Data(),"READ");
  TTree * sstr = (TTree*) infile->Get("sstr");

  //TString xStr = "(x_{cell}-x_{#gamma}^{inc})/d_{L}";
  TString xStr = "x^{cell i}_{center}-x_{#gamma}^{inc.}";
  TString yStr = "E_{i}/#sum_{i}E_{i}";

  Int_t thetaLb = 0;
  Int_t thetaUb = 30;
  Int_t thetaStep = 5;
  Int_t thetaCurr = thetaLb;

  Int_t nbins = 50;
  //TH2F * ssdist = new TH2F("ssdist","ssdist",nbins,-1.5,4.5,nbins,0,1);
  TH2F * ssdist = new TH2F("ssdist","ssdist",nbins,-5,20,nbins,0,1);
  TProfile * ssdist_pfx;

  TCanvas * sscanv = new TCanvas("sscanv","sscanv",800,800);
  sscanv->SetGrid(1,1);

  TGraphErrors * ssmeanVtheta = new TGraphErrors();
  TString ssmeanVthetaT = Form("<%s> vs #theta_{#gamma}",xStr.Data());
  ssmeanVtheta->SetTitle(ssmeanVthetaT.Data());
  Int_t ssmeanVtheta_k = 0;

  Float_t cellWidth;
  switch(lsvar0) {
    case 0: cellWidth = 5.81; break;
    case 1: cellWidth = 3.82; break;
    default: fprintf(stderr,"error: invalid lsvar0\n"); return;
  };

  //TString drawStr = Form("frac_edep:distx/%f",cellWidth);
  TString drawStr = Form("frac_edep:distx");

  TString cutStr_usu = Form("abs(energy-%f)<0.1 && lsvar==%d && hvvar==%d && frac_edep>%f",
                            energy0,lsvar0,hvvar0,frac_edep_cut);
  TString cutStr,ssdistT,ssdistN,pdfname;


  while(thetaCurr <= thetaUb) {
    ssdistT = 
      Form("%s vs. %s for E_{#gamma}=%.0f GeV and #theta_{#gamma}=%d deg",
           yStr.Data(),xStr.Data(),energy0,thetaCurr);
    ssdistN = Form("ssdist_th%d",(Int_t)thetaCurr);
    cutStr = cutStr_usu + Form(" && abs(theta-%d)<0.1",thetaCurr);
    sstr->Project("ssdist",drawStr.Data(),cutStr.Data());
    ssdist_pfx = ssdist->ProfileX();
    ssdist_pfx->SetTitle(ssdistT.Data());
    ssdist_pfx->SetName(ssdistN.Data());
    ssdist_pfx->Draw();

    outfile->cd();
    ssdist_pfx->Write();
    infile->cd();

    ssdist_pfx->GetYaxis()->SetRangeUser(0,1);

    pdfname = "ssplots.pdf";
    if(thetaCurr==thetaLb) pdfname = pdfname + "(";
    //else if(thetaCurr==thetaUb) pdfname = pdfname + ")";
    sscanv->Print(pdfname.Data(),"pdf");

    ssmeanVtheta->SetPoint(ssmeanVtheta_k,thetaCurr,ssdist_pfx->GetMean());
    //ssmeanVtheta->SetPointError(ssmeanVtheta_k,0,ssdist_pfx->GetRMS());
    ssmeanVtheta_k++;

    thetaCurr += thetaStep;
  };

  ssmeanVtheta->Fit("pol1","","",thetaLb,thetaUb);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);

  ssmeanVtheta->SetMarkerStyle(kFullCircle);
  ssmeanVtheta->Draw("APE");
  pdfname = pdfname + ")";
  sscanv->Print(pdfname.Data(),"pdf");
};






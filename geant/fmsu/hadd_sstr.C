// combines shower shape trees together, adding branches which include information
// about the initial thrown particle
//
// output file is gFiles/all.root

void hadd_sstr(TString dirname = "gFiles") {
  Int_t evtIdx;
  Int_t nstbTr,rowTr,colTr;
  Float_t total_edepTr;
  Float_t total_ncerTr;
  Int_t lsvar,hvvar;
  Float_t xcm,ycm,distx,disty,x_global,y_global;
  Float_t frac_edep,frac_ncer;
  Float_t showerMaxE,showerMaxP;

  TString outfilen = dirname + "/all.root";
  TFile * outfile = new TFile(outfilen.Data(),"RECREATE");
  TTree * sstr = new TTree("sstr","sstr");
  sstr->Branch("evtIdx",&evtIdx,"evtIdx/I");
  sstr->Branch("nstbTr",&nstbTr,"nstbTr/I");
  sstr->Branch("rowTr",&rowTr,"rowTr/I");
  sstr->Branch("colTr",&colTr,"colTr/I");
  sstr->Branch("total_edepTr",&total_edepTr,"total_edepTr/F");
  sstr->Branch("total_ncerTr",&total_ncerTr,"total_ncerTr/F");
  sstr->Branch("lsvar",&lsvar,"lsvar/I");
  sstr->Branch("hvvar",&hvvar,"hvvar/I");
  sstr->Branch("xcm",&xcm,"xcm/F");
  sstr->Branch("ycm",&ycm,"ycm/F");
  sstr->Branch("distx",&distx,"distx/F");
  sstr->Branch("disty",&disty,"disty/F");
  sstr->Branch("x_global",&x_global,"x_global/F");
  sstr->Branch("y_global",&y_global,"y_global/F");
  sstr->Branch("frac_edep",&frac_edep,"frac_edep/F");
  sstr->Branch("frac_ncer",&frac_ncer,"frac_ncer/F");
  sstr->Branch("showerMaxE",&showerMaxE,"showerMaxE/F");
  sstr->Branch("showerMaxP",&showerMaxP,"showerMaxP/F");

  Float_t energy,theta,phi;
  Int_t posIdx;
  sstr->Branch("energy",&energy,"energy/F");
  sstr->Branch("theta",&theta,"theta/F");
  sstr->Branch("phi",&phi,"phi/F");
  sstr->Branch("posIdx",&posIdx,"posIdx/I");




  TSystemDirectory dir(dirname.Data(), dirname.Data());
  TList *files = dir.GetListOfFiles();
  TString fname,fnamemod;


  TString full_fname;
  TFile * file_curr;
  TTree * sstr_curr;

  if (files) {
    TSystemFile *file;
    TIter next(files);
    while ((file=(TSystemFile*)next())) {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.BeginsWith("gFile_") && fname.EndsWith(".root")) {
        fnamemod = fname;
        full_fname = dirname+"/"+fname;
        fnamemod.ReplaceAll("gFile_","");
        fnamemod.ReplaceAll(".root","");
        fnamemod.ReplaceAll("_"," ");

        sscanf(fnamemod.Data(),"e%f th%f ph%f pos%d",&energy,&theta,&phi,&posIdx);

        cout << "reading " << fname.Data() << endl;
        //cout << fname.Data() << "   |" << fnamemod.Data() << "|" << endl;
        //printf("    energy=%f theta=%f phi=%f posIdx=%d\n",energy,theta,phi,posIdx);

        file_curr = new TFile(full_fname.Data(),"READ");
        sstr_curr = (TTree*) file_curr->Get("sstr");

        sstr_curr->SetBranchAddress("evtIdx",&evtIdx);
        sstr_curr->SetBranchAddress("nstbTr",&nstbTr);
        sstr_curr->SetBranchAddress("rowTr",&rowTr);
        sstr_curr->SetBranchAddress("colTr",&colTr);
        sstr_curr->SetBranchAddress("total_edepTr",&total_edepTr);
        sstr_curr->SetBranchAddress("total_ncerTr",&total_ncerTr);
        sstr_curr->SetBranchAddress("lsvar",&lsvar);
        sstr_curr->SetBranchAddress("hvvar",&hvvar);
        sstr_curr->SetBranchAddress("xcm",&xcm);
        sstr_curr->SetBranchAddress("ycm",&ycm);
        sstr_curr->SetBranchAddress("distx",&distx);
        sstr_curr->SetBranchAddress("disty",&disty);
        sstr_curr->SetBranchAddress("x_global",&x_global);
        sstr_curr->SetBranchAddress("y_global",&y_global);
        sstr_curr->SetBranchAddress("frac_edep",&frac_edep);
        sstr_curr->SetBranchAddress("frac_ncer",&frac_ncer);
        sstr_curr->SetBranchAddress("showerMaxE",&showerMaxE);
        sstr_curr->SetBranchAddress("showerMaxP",&showerMaxP);

        for(int x=0; x<sstr_curr->GetEntries(); x++) {
          sstr_curr->GetEntry(x);
          sstr->Fill();
        };
      };
    };
  };

  outfile->cd();
  sstr->Write();
};

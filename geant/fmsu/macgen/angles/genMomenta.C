// generates momentum vectors (constrained pz=1);
// for given ranges in theta and phi

void genMomenta()
{
  system("touch anglist.dat; rm anglist.dat");
  const Float_t PI = 3.141592;

  // theta
  Float_t theta_l = 30e-3;
  Float_t theta_inc = 30e-3;
  Float_t theta_h = 210e-3;

  // phi
  Float_t phi_l = 0;
  Float_t phi_inc = PI/12;
  Float_t phi_h = 2*PI - phi_inc;

  Float_t phi = phi_l;
  Float_t theta = theta_l;
  Float_t px,py,pz,pt,p;
  Float_t eta;
  pz = 1;
  
  TTree *tr = new TTree("tr","tr");
  tr->Branch("theta",&theta); // polar angle
  tr->Branch("phi",&phi); // azimuth
  tr->Branch("eta",&eta); // psuedorapidity
  tr->Branch("p",&p); // momentum norm
  tr->Branch("pt",&pt); // transverse momentum norm
  tr->Branch("px",&px); 
  tr->Branch("py",&py); 
  tr->Branch("pz",&pz);
  
  gSystem->RedirectOutput("anglist.dat","a");
  printf("0 0 1\n");
  gSystem->RedirectOutput(0);
  while(theta <= theta_h)
  {
    printf("theta=%f\n",theta);
    while(phi <= phi_h)
    {
      printf("  phi=%f\n",phi);
      pt = tan(theta);
      px = pt*cos(phi);
      py = pt*sin(phi);
      p = sqrt(pow(px,2)+pow(py,2)+1);
      eta = -log(tan(theta/2));
      tr->Fill();
      gSystem->RedirectOutput("anglist.dat","a");
      printf("%f %f %f\n",px,py,pz);
      gSystem->RedirectOutput(0);
      phi += phi_inc;
    };
    theta += theta_inc;
    phi = phi_l;
  };
  
  TFile *outfile = new TFile("momenta.root","RECREATE");
  tr->Write();
};



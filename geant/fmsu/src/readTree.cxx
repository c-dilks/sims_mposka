#include "TFile.h"
#include "TTree.h"
#include "Riostream.h"

Float_t new_x = 0;
Float_t new_y = 0;

void readTree(){
  TFile *fName = new TFile("*.root");//reading the file
  TTree *testtree = (TTree*)fName->Get("testtree"); // new TTree("t","holder tree");//create a tree to read

  Float_t px[100]={0};
  Float_t py[100]={0};
  Float_t pz[100]={0};
  Float_t vx[100]={0};
  Float_t vy[100]={0};
  Float_t vz[100]={0};
  Float_t energy[100]={0};
  //declearing arrays to hold the data
  //link the array to the tree

  testtree->SetBranchAddress("vx",&vx);
  testtree->SetBranchAddress("vy",&vy);
  testtree->SetBranchAddress("vz",&vz);
  testtree->SetBranchAddress("px",&px);
  testtree->SetBranchAddress("py",&py);
  testtree->SetBranchAddress("pz",&pz);
  testtree->SetBranchAddress("energy",&energy);
 
  TTree* tf = new TTree("tf","friend tree");//create a friend tree
  tf->Branch("projX", &new_x,"projX/F");
  tf->Branch("projY", &new_y,"projY/F");

  Float_t z_input;

  cout << "Please input the Z position for the projection: " <<endl;
  cin >> z_input;


  Int_t num = testtree->GetEntries();
  cout<<"number of entries in testtree is "<<num<<"."<<endl;
  Int_t j=0;
  for(j;j<num; j++)
  {
     testtree->GetEntry(j);
     projection(vx[0],vy[0],vz[0],px[0],py[0],pz[0],z_input,0);
     cout<<"new_x="<<new_x<<" && J="<<j<<endl;
     tf->Fill();// fill the friend tree
  }

  //



  //  f.cd();
  tf->Write();
  testtree->AddFriend("tf");



  for (int i = 0;i<100;i++){
    vx[i]=0;
    vy[i]=0;
    vz[i]=0;
    px[i]=0;
    py[i]=0;
    pz[i]=0;
  }//clean the holder array
  


}

void  projection(Float_t vx, Float_t vy, Float_t vz, Float_t px, Float_t py, Float_t pz, Float_t z_input,Int_t j)
{
  Float_t tmp_r =0, tmp_x=0, tmp_y=0;
  tmp_r = (z_input-vz)/pz;
  tmp_x = (tmp_r *px) + vx;
  tmp_y = (tmp_r *py) + vy;
 
  new_x = tmp_x;
  new_y = tmp_y;

}




 /*
 cout << "At location Z = " << z_input << " the first particle will be at location:" <<endl;
  cout << "x = " <<  new_x << endl;
  cout << "y = " <<  new_y << endl; 
 */

/*

void sync(TTree *t1, TTree *t2){
  if(t1->fCacheSize() > t2->fCacheSize()){
  
  }
  else {
  
  }
}

*/

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

namespace {
  enum ls_enum {kL,kS}; // large/small
  enum hv_enum {kH,kV}; // horiz/vert
}

PostSim::PostSim()
{
	MatrixArray.SetOwner((Bool_t) true);
	evpos.SetOwner((Bool_t) true);

	numberMatrices = 10; //Number of Matrices. Note, nMatrices also exists. Fix this

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
	//Matt
        x_global=0; 
        y_global=0;
	ShowerMaxLarge=0;
	ShowerMaxSmall=0;
	//	
	strcpy(name,"");
	isOpt=0;
	isHit=0;
	trNum=0;
	cosTh=0;
	NCer=0;
	ltime=0;
	gtime=0;
	arr_size=0;
	Emod=0; //BS

	// instantiate p_files and QT
	//  fu = new G4FileUtilities();
	FMSGAIN = fu.GetEnv("FMSGAIN");
	FMSCORR = fu.GetEnv("FMSCORR");
	QTMAP = fu.GetEnv("QTMAP");
	QTMAP2PP = fu.GetEnv("QTMAP2PP");
	FMSTXT = fu.GetEnv("FMSTXT");
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
	pnqtEnergy=&nqtEnergy;
	pnqtEnergyMod=&nqtEnergyMod;
	pnqtRadDamage=&nqtRadDamage;

        TString shshN,shshT;
        for(int ls=0; ls<2; ls++) {
          for(int hv=0; hv<2; hv++) {
            shshN = Form("shshEdep%s%s",ls==kL?"L":"S",hv==kH?"H":"V");
            shshT = Form("%s Cells %s E_{dep} Shower Shape;%s",
              ls==kL?"Large":"Small",hv==kH?"Horizontal":"Vertical",hv==kH?"x_{c}":"y_{c}");
            shsh_edep[ls][hv] = new TH2F(shshN.Data(),shshT.Data(),100,-5,5,100,0,1);

            shshN = Form("shshNcer%s%s",ls==kL?"L":"S",hv==kH?"H":"V");
            shshT = Form("%s Cells %s Cherenkov Shower Shape;%s",
              ls==kL?"Large":"Small",hv==kH?"Horizontal":"Vertical",hv==kH?"x_{c}":"y_{c}");
            shsh_ncer[ls][hv] = new TH2F(shshN.Data(),shshT.Data(),100,-5,5,100,0,1);
          };
        };
        
        sstr = new TTree("sstr","sstr");
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

        evtIdx=0;
}

PostSim::~PostSim()
{
	if (QT) delete QT;
	if (p_files) delete p_files;
}


// adds current gtree entry to appropriate matrices/objarrays
void PostSim::FillMatrices(int i, bool t)
{
	TMatrix ** sfadc = (TMatrix**) MatrixArray.At(0);
	TMatrix ** sedep = (TMatrix**) MatrixArray.At(1);
	TMatrix ** shits = (TMatrix**) MatrixArray.At(2);
	TMatrix ** snopt = (TMatrix**) MatrixArray.At(3);
	TMatrix ** sncer = (TMatrix**) MatrixArray.At(4);
	TMatrix ** sadc = (TMatrix **) MatrixArray.At(5);
	TMatrix ** sfadcM = (TMatrix **) MatrixArray.At(6);
	TMatrix ** sedepM = (TMatrix **) MatrixArray.At(7);
	TMatrix ** radhits = (TMatrix **) MatrixArray.At(8);
	TMatrix ** radadc = (TMatrix **) MatrixArray.At(9);


	TMatrix ** fadc1 = (TMatrix**) MatrixArray.At(i*numberMatrices);
	TMatrix ** sedep1 = (TMatrix**) MatrixArray.At(i*numberMatrices+1);
	TMatrix ** hits1 = (TMatrix**) MatrixArray.At(i*numberMatrices+2);
	TMatrix ** nopt1 = (TMatrix**) MatrixArray.At(i*numberMatrices+3);
	TMatrix ** ncer1 = (TMatrix**) MatrixArray.At(i*numberMatrices+4);
	TMatrix ** adc1 = (TMatrix**) MatrixArray.At(i*numberMatrices+5);
	TMatrix ** fadc1M = (TMatrix**) MatrixArray.At(i*numberMatrices+6);
	TMatrix ** sedep1M = (TMatrix**) MatrixArray.At(i*numberMatrices+7);
	TMatrix ** radhits1 = (TMatrix**) MatrixArray.At(i*numberMatrices+8);
	TMatrix ** radadc1 = (TMatrix**) MatrixArray.At(i*numberMatrices+8);

	if(t){
		(*(sedepM[nstb-1]))(row,col) += Emod;
		(*(sedep[nstb-1]))(row,col) += en;
		(*(sncer[nstb-1]))(row,col) += NCer;
		if(isOpt)
		{
			(*(snopt[nstb-1]))(row,col) += 1;
			if(isHit) (*(shits[nstb-1]))(row,col) += 1;
			if(isRealHit) (*(radhits[nstb-1])) (row,col) +=1;
		};
	}
	(*(sedep1M[nstb-1]))(row,col) += Emod;
	(*(sedep1[nstb-1]))(row,col) += en;
	(*(ncer1[nstb-1]))(row,col) += NCer;
	if(isOpt)
	{
		(*(nopt1[nstb-1]))(row,col) += 1;
		if(isHit)  (*(hits1[nstb-1]))(row,col) += 1;
		if(isRealHit)  (*(radhits1[nstb-1]))(row,col) += 1;
	};
}

void PostSim::FillMatrixArray(int m, Int_t n[])
{
	for(int i=0;i<=m;i++)
	{
		for(Int_t j=0; j<numberMatrices; j++)
		{
			MatrixArray.AddLast((TObject*) new TMatrix[4]);
		}
	}
	for(int i=0;i<=m;i++){
		/*BS. Possible change to TMatrix*  fadc1[4] */
		TMatrix ** fadc1 = (TMatrix**) MatrixArray.At(i*numberMatrices);
		TMatrix ** sedep1 = (TMatrix**) MatrixArray.At(i*numberMatrices+1);
		TMatrix ** hits1 = (TMatrix**) MatrixArray.At(i*numberMatrices+2);
		TMatrix ** nopt1 = (TMatrix**) MatrixArray.At(i*numberMatrices+3);
		TMatrix ** ncer1 = (TMatrix**) MatrixArray.At(i*numberMatrices+4);
		TMatrix ** adc1 = (TMatrix**) MatrixArray.At(i*numberMatrices+5);
		TMatrix ** fadc1M = (TMatrix**) MatrixArray.At(i*numberMatrices+6);
		TMatrix ** sedep1M = (TMatrix**) MatrixArray.At(i*numberMatrices+7);
		TMatrix ** radhits = (TMatrix**) MatrixArray.At(i*numberMatrices+8);
		TMatrix ** radadc = (TMatrix**) MatrixArray.At(i*numberMatrices+9);

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
			fadc1M[n] = new TMatrix(nnr,nnc);
			sedep1M[n] = new TMatrix(nnr,nnc);
			radhits[n] = new TMatrix(nnr,nnc);
			radadc[n] = new TMatrix(nnr,nnc);
		};
	}
	ZeroMatrices(m); //BS. Method in PostSim. Sets each element of the above list of matrices to 0 with a for loop.
}

// sets all elements of all private matrices to zero
void PostSim::ZeroMatrices(int m)
{
	int nrows,ncols;
	for(int i=0;i<=m;i++){
		TMatrix ** fadc0 = (TMatrix**) MatrixArray.At(i*numberMatrices);
		TMatrix ** sedep0 = (TMatrix**) MatrixArray.At(i*numberMatrices+1);
		TMatrix ** hits0 = (TMatrix**) MatrixArray.At(i*numberMatrices+2);
		TMatrix ** nopt0 = (TMatrix**) MatrixArray.At(i*numberMatrices+3);
		TMatrix ** ncer0 = (TMatrix**) MatrixArray.At(i*numberMatrices+4);
		TMatrix ** adc0 = (TMatrix**) MatrixArray.At(i*numberMatrices+5);
		TMatrix ** fadc0M = (TMatrix**) MatrixArray.At(i*numberMatrices+6);
		TMatrix ** sedep0M = (TMatrix**) MatrixArray.At(i*numberMatrices+7);
		TMatrix ** radhits0 = (TMatrix**) MatrixArray.At(i*numberMatrices+8);
		TMatrix ** radadc0 = (TMatrix**) MatrixArray.At(i*numberMatrices+9);
		for(int n=0; n<4; n++)
		{
			nrows=34; ncols=17;
			if(n>1) { nrows=24; ncols=12; };
			for(int nr=0; nr<nrows; nr++)
			{
				for(int nc=0; nc<ncols; nc++)
				{
					(*(fadc0[n]))(nr,nc)=0;
					(*(sedep0[n]))(nr,nc)=0;
					(*(hits0[n]))(nr,nc)=0;
					(*(nopt0[n]))(nr,nc)=0;
					(*(ncer0[n]))(nr,nc)=0;
					(*(adc0[n]))(nr,nc)=0;
					(*(fadc0M[n]))(nr,nc)=0;
					(*(sedep0M[n]))(nr,nc)=0;
					(*(radhits0[n]))(nr,nc)=0;
					(*(radadc0[n]))(nr,nc)=0;
				}
			}
		}
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
	/*
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
	 */

	Double_t edeptot=0;
	Double_t efittot=0;
	Double_t eradtot=0;
	Double_t radhitstot=0;

	ppL[0] = 0;
	ppL[1] = 0.00221733; //BS, linear slope. Average of values
	ppL[2] = 0;
	ppL[3] = 0;
	ppL[4] = 0; 
	ppS[0] = 0;
	ppS[1] = 0.00221733;
	ppS[2] = 0; 
	ppS[3] = 0;
	ppS[4] = 0; 

	Int_t nMatrices=10;
	TMatrix ** sfadc = (TMatrix**) MatrixArray.At(k*nMatrices);
	TMatrix ** sedep = (TMatrix**) MatrixArray.At(k*nMatrices+1);
	TMatrix ** shits = (TMatrix**) MatrixArray.At(k*nMatrices+2);
	TMatrix ** snopt = (TMatrix**) MatrixArray.At(k*nMatrices+3);
	TMatrix ** sncer = (TMatrix**) MatrixArray.At(k*nMatrices+4);
	TMatrix ** sadc = (TMatrix**) MatrixArray.At(k*nMatrices+5);
	TMatrix ** sfadcM = (TMatrix**) MatrixArray.At(k*nMatrices+6);
	TMatrix ** sedepM = (TMatrix**) MatrixArray.At(k*nMatrices+7);
	TMatrix ** radhits = (TMatrix**) MatrixArray.At(k*nMatrices+8);
	TMatrix ** radadc = (TMatrix**) MatrixArray.At(k*nMatrices+9);
	TMatrix ** adcgeo1 = (TMatrix**) MatrixArray.At(k*nMatrices+10);
	TMatrix ** adcgeo2 = (TMatrix**) MatrixArray.At(k*nMatrices+11);
	TMatrix ** adcgeo3 = (TMatrix**) MatrixArray.At(k*nMatrices+12);
	TMatrix ** adcgeo4 = (TMatrix**) MatrixArray.At(k*nMatrices+13);


	for(Int_t r=0; r<shits[nn]->GetNrows(); r++)
	{
		for(Int_t c=0; c<shits[nn]->GetNcols(); c++)
		{
			hits = (*(shits[nn]))(r,c);
			edep = (*(sedep[nn]))(r,c);
			edepMod= (*(sedepM[nn]))(r,c);
			Float_t tempRadHits = (*(radhits[nn]))(r,c);
			radhitstot+= tempRadHits;


			if(hits>0 || edep>0)
			{
				gain_ = cellgeo->GetGain(nn+1,r,c);
				corr_ = cellgeo->GetCorr(nn+1,r,c);
				gaincorr = gain_*corr_;

				gain1=geo1.GetGain(nn+1,r,c);
				corr1=geo1.GetCorr(nn+1,r,c);
				gaincorr1=gain1*corr1;

				gain2=geo2.GetGain(nn+1,r,c);
				corr2=geo2.GetCorr(nn+1,r,c);
				gaincorr2=gain2*corr2;

				gain3=geo3.GetGain(nn+1,r,c);
				corr3=geo3.GetCorr(nn+1,r,c);
				gaincorr3=gain3*corr3;

				gain4=geo4.GetGain(nn+1,r,c);
				corr4=geo4.GetCorr(nn+1,r,c);
				gaincorr4=gain4*corr4;

				if(nn<2)
				{
					fac=17;
					fit_edep = ppL[0]+
						ppL[1]*hits+
						ppL[2]*pow(hits,2)+
						ppL[3]*pow(hits,3)+
						ppL[4]*pow(hits,4);

					fitRadEdep = ppL[1] * tempRadHits;
				}
				else
				{
					fac=12;
					fit_edep = ppS[0]+
						ppS[1]*hits+
						ppS[2]*pow(hits,2)+
						ppS[3]*pow(hits,3)+
						ppS[4]*pow(hits,4);

					fitRadEdep = ppS[1] * tempRadHits;
				};
				adc = (Int_t)(fit_edep/gaincorr + 0.5); // round to nearest integer
				adcrad = (Int_t) (fitRadEdep/gaincorr + 0.5);
				fadc= (Int_t) ((edep)/(gaincorr) + 0.5);
				fadcMod=(Int_t) ( edepMod/(gaincorr) + 0.5);
				chan = c+r*fac+1;


				(*(sadc[nn]))(r,c) = adc;
				(*(sfadc[nn]))(r,c)= fadc;
				(*(sfadcM[nn]))(r,c)=fadcMod;
				(*(radadc[nn]))(r,c) = adcrad;

				nstb_tr = nn+1;
				row_tr = r;
				col_tr = c;

				edeptot+= edep;
				efittot+= fit_edep;
				eradtot+= fitRadEdep;
			};
		};
	};

	if(nn == 3)
	{
		for(int g=0; g<3700; g++) {QTdata[g]=0; QtEnergy[g]=0; QtEnergyMod[g]=0; QtGeo1[g]=0; QtGeo2[g]=0; QtGeo3[g]=0; QtGeo4[g]=0;}
		success = QT->encodeQT(sadc,nQTdataPointer,QTdata);
		Bool_t DidEncode= QT->encodeQT(sfadc,pnqtEnergy,QtEnergy);
		Bool_t DidEncodeMod= QT->encodeQT(sfadcM,pnqtEnergyMod,QtEnergyMod);
		Bool_t DidEncodeRadDamage= QT->encodeQT(radadc,pnqtRadDamage,QtRadDamage);

		nQTdata = *nQTdataPointer;
		nqtEnergy=*pnqtEnergy;
		nqtEnergyMod=*pnqtEnergyMod;
		nqtRadDamage=*pnqtRadDamage;
	}
}

// loops through edep matrices and sets nstb_ht,row_ht,col_ht to
// the highest tower corrdinates
void PostSim::GetHT(int i)
{
	TMatrix ** sedep = (TMatrix**) MatrixArray.At(i*numberMatrices+1);
	max_edep=0;
	nstb_ht=row_ht=col_ht=0;

	for(int n=0; n<4; n++)
	{
		for(int r=0; r<(sedep[n])->GetNrows(); r++)
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


// fill shower shape histos
void PostSim::FillShowerShape() {

  Bool_t debugSS = true;
  if(debugSS) printf("\n\n\n[ V V V V ] SHOWER SHAPE [ V V V V ] \n\n");

  // trabaja -- using "sedep" and "sncer"... is this correct? do we need to use sedep1? or sedepM?
  TMatrix ** sedep = (TMatrix**) MatrixArray.At(1); 
  TMatrix ** sncer = (TMatrix**) MatrixArray.At(4);


  if(debugSS) {
    for(int ns=0; ns<4; ns++) {
      printf(">>> sedep %d @ %p dim=%dx%d\n",ns,(void*)sedep[ns],sedep[ns]->GetNrows(),sedep[ns]->GetNcols());
      //sedep[ns-1]->Print();
      printf(">>> sncer %d @ %p dim=%dx%d\n",ns,(void*)sncer[ns],sedep[ns]->GetNrows(),sedep[ns]->GetNcols());
      //sncer[ns-1]->Print();
    };
    printf("HT: nstb_ht=%d row_ht=%d col_ht=%d\n",nstb_ht,row_ht,col_ht);
    printf("x_global=%f y_global=%f\n",x_global,y_global);
  };

  for(int ls=0; ls<2; ls++) {
    total_edep[ls] = 0;
    total_ncer[ls] = 0;
  };
  rowmax[kL] = 34;
  rowmax[kS] = 24;
  colmax[kL] = 17;
  colmax[kS] = 12;


  // compute totals
  for(int ns=0; ns<4; ns++) {
    lsvar = ns<2 ? kL:kS;
    for(int rs=0; rs<rowmax[lsvar]; rs++) {
      for(int cs=0; cs<colmax[lsvar]; cs++) {
        total_edep[lsvar] += (*(sedep[ns]))(rs,cs);
        total_ncer[lsvar] += (*(sncer[ns]))(rs,cs);
      };
    };
  };

  // fill shower shape (for large XOR small cells, depening on which has HT)
  Bool_t sstrFilled = false;
  for(int ns=0; ns<4; ns++) {
    lsvar = ns<2 ? kL:kS;
    if(lsvar == ((nstb_ht-1)<2 ? kL:kS)) {
      for(int rs=0; rs<rowmax[lsvar]; rs++) {
        for(int cs=0; cs<colmax[lsvar]; cs++) {

          frac_edep = 0;
          frac_ncer = 0;

          total_edepTr = total_edep[lsvar];
          total_ncerTr = total_ncer[lsvar];

          if(total_edepTr>0) frac_edep = (*(sedep[ns]))(rs,cs) / total_edepTr;
          if(total_ncerTr>0) frac_ncer = (*(sncer[ns]))(rs,cs) / total_ncerTr;

          nstbTr = ns;
          rowTr = rs;
          colTr = cs;

          GetCellCenter(ns,rs,cs,xcm,ycm);

          distx = xcm - x_global;
          disty = ycm - y_global;

          if(debugSS) printf("ns=%d rs=%d cs=%d\n",ns,rs,cs);
          if(rs==row_ht) { 
            if(debugSS) printf("  rs=%d distx=%f frac_edep=%f\n",rs,distx,frac_edep);
            hvvar = kH;
            if(frac_edep>0) shsh_edep[lsvar][hvvar]->Fill(distx,frac_edep);
            if(frac_ncer>0) shsh_ncer[lsvar][hvvar]->Fill(distx,frac_ncer);
            sstr->Fill();
            sstrFilled = true;
          };

          if(cs==col_ht) { 
            if(debugSS) printf("  cs=%d disty=%f frac_edep=%f\n",cs,disty,frac_edep);
            hvvar = kV;
            if(frac_edep>0) shsh_edep[lsvar][hvvar]->Fill(disty,frac_edep);
            if(frac_ncer>0) shsh_ncer[lsvar][hvvar]->Fill(disty,frac_ncer);
            sstr->Fill();
            sstrFilled = true;
          };
        };
      };
    };
  }; // eo fill shower shape loop

  if(sstrFilled) evtIdx++;

  if(debugSS) printf("\n\n\n[ ^ ^ ^ ^ ] SHOWER SHAPE [ ^ ^ ^ ^ ] \n\n");
  return;
};


void PostSim::GetCellCenter(int n0,int r0,int c0,Float_t &xcm0,Float_t &ycm0) {
  switch(n0) {
    case 0:
      xcm0 = -(c0+0.5) * 5.81;
      ycm0 =  (16.5-r0) * 5.81;
      break;
    case 1:
      xcm0 =  (c0+0.5) * 5.81;
      ycm0 =  (16.5-r0) * 5.81;
      break;
    case 2:
      xcm0 = -(c0+0.5) * 3.82;
      ycm0 =  (11.5-r0) * 3.82;
      break;
    case 3:
      xcm0 =  (c0+0.5) * 3.82;
      ycm0 =  (11.5-r0) * 3.82;
      break;
  };
  return;
};

#ifndef PostSim_h
#define PostSim_h 1
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TMatrix.h"
#include "TObjArray.h"
#include "TString.h"
#include "CellGeo.hh"
#include "G4FileUtilities.hh"
#include "Qt.h" // must modify LDFLAGS and CPPFLAGS to compile with FMSSRC headers
#include "FilesSet.h"


#include <vector>
#include <iostream>

class PostSim
{
	public:
		PostSim();
		~PostSim();
		void FillMatrices(int i,bool t);
		void ZeroMatrices(int m);
		void Digitise(Int_t nstb_, int k);
		void GetHT(int i);
		void FillMatrixArray(int m, Int_t n[]);
                void FillShowerShape();
                void GetCellCenter(int n0,int r0,int c0,Float_t &xcm0,Float_t &ycm0);

		TCanvas * c1;
		TH2F * hh[4];


		Int_t eventnumber;
		int number;
		Int_t numberMatrices;
		double ox;
		double oy;
		G4int photid;

		int evnum;      // event number
		int nstb;       // fms coords
		int row;        //   |
		int col;        //   |
		float gain;     // pmt gain
		float corr;     // pmt gain correction
		float en;       // deposited energy
		float tot_en;   // total energy ??
		float kin;      // kinetic energy
		float verx;     // vertex location;
		float very;     //   |
		float verz;     //   |
		float px;       // normalised momentum vector at vertex
		float py;       //   |
		float pz;       //   |
		char name[32];  // particle name
		bool isOpt;     // true if photon is optical
		bool isHit;     // true if photon hits photocathode 
		int trNum;      // track id
		float cosTh;    // angle of incidence (for photons)
		int NCer;       // number of cherenkov photons
		int ltime;      // local time
		int gtime;      // global time

		int nstb_ht;    // highest tower coordinates
		int row_ht;
		int col_ht;
		int nprimaries;
		int originalID;

		//Matt
		float ShowerMaxLarge;
		float ShowerMaxSmall;
                float x_global;
                float y_global;
		//	

		TObjArray MatrixArray;
		TObjArray evpos;

		//BS
		Int_t didPassTriggers; //BS
		Bool_t TriggersPassed[9]; // 3 small board sums, 3 large board sums, then 3 jet patches, each ordered from least to most
		Bool_t isRealHit; //method of including radiation damage, is true if its a photon that would hit the photocathode even after damage is considered

		/*    TObjArray * fadc_arr[4]; // edep/(gain*corr)
		      TObjArray * edep_arr[4]; // edep
		      TObjArray * hits_arr[4]; // number of hits
		      TObjArray * nopt_arr[4]; // number of optical photons
		      TObjArray * ncer_arr[4]; // number cherenkov photons
		      TObjArray * adc_arr[4]; // adc counts (via digitisation)

		      TH2F * egraph_cells_L; // energy deposition vs. hits (large cells) 
		      TH2F * egraph_integ_L; // energy deposition vs. hits (large integrated)
		      TH2F * egraph_cells_S; // energy deposition vs. hits (small cells)
		      TH2F * egraph_integ_S; // energy deposition vs. hits (small integrated)
		 */
		Int_t nstb_tr,row_tr,col_tr,chan;
		Float_t hits,edep,fit_edep,gain_,corr_,gaincorr, fitRadEdep;
		Int_t adc, adcrad;
		TTree * ttr; // main tree

		UInt_t nQTdata;
		UInt_t *nQTdataPointer;
		UInt_t QTdata[3700];
		//Int_t crate[3700];
		Qt * QT;

		FilesSet * p_files;

		TTree * hsimu;
		//TMatrix * adc_mat[4];

		//BS
		std::vector <TMatrix*> testvec;
		UInt_t dummyFirstInEvent;
		//    TMatrix * fadc_mat[4]; //Moved here to access in EventAction
		Double_t zdepth;
		float Emod;
		//    TMatrix * fadc_mat_mod[4]; //with adjustment on energy
		Float_t edepMod;

		UInt_t QtEnergy[3700];
		UInt_t nqtEnergy;
		UInt_t *pnqtEnergy;

		UInt_t QtEnergyMod[3700];
		UInt_t nqtEnergyMod;
		UInt_t *pnqtEnergyMod;

		UInt_t QtRadDamage[3700];
		UInt_t nqtRadDamage;
		UInt_t *pnqtRadDamage;
		//    TMatrix * edepMod_mat[4];

		Int_t fadc;
		Int_t fadcMod;


		///////BS
		CellGeo geo1, geo2, geo3, geo4;
		G4float gain1, gain2, gain3, gain4;
		G4float corr1, corr2, corr3, corr4;
		G4float gaincorr1, gaincorr2, gaincorr3, gaincorr4;
		Int_t adcGeoValue1, adcGeoValue2, adcGeoValue3, adcGeoValue4;

		UInt_t QtGeo1[3700];
		UInt_t nqtGeo1;
		UInt_t *pnqtGeo1;

		UInt_t QtGeo2[3700];
		UInt_t nqtGeo2;
		UInt_t *pnqtGeo2;

		UInt_t QtGeo3[3700];
		UInt_t nqtGeo3;
		UInt_t *pnqtGeo3;

		UInt_t QtGeo4[3700];
		UInt_t nqtGeo4;
		UInt_t *pnqtGeo4;

		Int_t Trig1[9], Trig2[9], Trig3[9], Trig4[9];
		//////End BS


                TTree * sstr;
                TH2F * shsh_edep[2][2]; // shower shape [0=large, 1=small] [0=horizontal, 1=vertical]
                TH2F * shsh_ncer[2][2]; 


                Float_t showerMaxE;
                Float_t showerMaxP;

	private:
		int arr_size;
		Int_t nnr,nnc;
		//    TMatrix * edep_mat[4];
		//    TMatrix * hits_mat[4];
		//    TMatrix * nopt_mat[4];
		//    TMatrix * ncer_mat[4];
		Float_t ppL[5];
		Float_t ppS[5];
		Float_t hits_tot,edep_tot;
		Int_t fac;

		float max_edep,max_edep_tmp;

		Bool_t success;

		G4FileUtilities fu;
		G4String FMSTXT;
		G4String FMSGAIN;
		G4String FMSCORR;
		G4String QTMAP;
		G4String QTMAP2PP;

                
                // for shower max algorithm
                Int_t evtIdx;
                Int_t nstbTr,rowTr,colTr;
                Float_t total_edep[2]; // [ls]
                Float_t total_ncer[2];
                Int_t rowmax[2];
                Int_t colmax[2];
                Float_t total_edepTr;
                Float_t total_ncerTr;
                Int_t lsvar,hvvar;
                Float_t xcm,ycm,distx,disty;
                Float_t a_edep,a_ncer;
                Float_t frac_edep,frac_ncer;
};

#endif

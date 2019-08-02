#include "TMath.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <sstream>
using namespace std;

struct t_fast{
    ULong64_t fNbOM=0;
    ULong64_t fNbPhoto=0;
    unsigned fNbDetected[3][100]; //[limit][boh]
};

inline bool file_exists (const std::string& name) {
	//struct stat buffer;   
	//return (stat (name.c_str(), &buffer) == 0); 
	ifstream f(name.c_str());
	return f.good();
}
void makeAAtable_newgrid(){
	cout << "AA 2D table." << endl;
	cout << "First row - fixed energies (eV)    at which the simulation is done" << endl;
	cout << "Second row - fixed wavelengths (nm) at which the simulation is done" << endl;
	cout << "Third row - AA values (cm^2) for cos(theta)=1" << endl;
	cout << "Forth row - AA values (cm^2) for cos(theta)=0.98 and so on (step -0.02 till -1)" << endl;

	double hc = 197.327;  //MeV*fm 1e6*1e-6  [ev*nm]
	double wlmin = 250;
	double wlmax = 750;
	double step = (wlmax-wlmin)/50.;
	TGraphErrors *gAA1 = new TGraphErrors();
	TGraphErrors *gAA2 = new TGraphErrors();
	TGraphErrors *gAA3 = new TGraphErrors();
	double aa2max = 0;
	double cosangle;
	double scale = TMath::Pi()*21.59*21.59/1e7;  //beam radius per number of generated photons per each scan cosangle
        double e;

	for (cosangle = 1.04; cosangle >= -1; cosangle-=0.02) { //-1;1 with 100 steps
		for (double wl = wlmin; wl <= wlmax; wl+=step) {
			e = 2*TMath::Pi()*hc/wl;
			if (cosangle == 1.04) {
				if (wl == wlmin) cout << "E[eV] ";
				cout << e << "\t";
			} else if (cosangle == 1.02) {
				if (wl == wlmin) cout << "wl[nm] ";
				cout << wl << "\t";
			} else {
				if (wl == wlmin) cout << cosangle << " ";
				ostringstream oss;

                                double angle = acos(cosangle)/TMath::Pi()*180.;
                                //string sangle = sprintf("%.4f", angle);
				/*
				if (cosangle==1) 
					oss << "../../build/prod/AA_DOM_lux0_0deg-" << fixed << setprecision(5) << e << "eV" << ".root";
				else if (cosangle==0)
					oss << "../../build/prod/AA_DOM_lux0_90deg-" << fixed << setprecision(5) << e << "eV" << ".root";
				else
					*/

				/*
				double angles[5] = {angle-0.0002, angle-0.0001,angle,angle+0.0001, angle+0.0002};
				double energies[5] = {e-0.00002, e-0.00001, e, e+0.00001, e+0.00002};
				bool found = false;
				for (int ii = 0; ii < 5; ii++) for (int jj = 0; jj < 5; jj++) {
					if (!found) {
						oss.str("");
						oss << "../../build/prod/AA_DOM_lux0_" << fixed << setprecision(4) << angles[ii] << "deg-" << fixed << setprecision(5) << energies[jj] << "eV" << ".root";
						if (file_exists(oss.str())) {
							found = true;
						}
					}
				} 
				*/

				oss << "../../buildnew/prod/DOMAA_lux0_" << fixed << setprecision(2) << cosangle << "-" << fixed << int(wl) << "nm" << ".root";
				//cout << angle << " " << e << " " << oss.str() << endl;
				if (!file_exists(oss.str())) {
					oss.str("");
					oss << "../../buildnew/prod/DOMAA_lux0_" << fixed << setprecision(2) << cosangle+0.001 << "-" << fixed << int(wl) << "nm" << ".root";
					f1= new TFile(oss.str().c_str());
				}
				TFile *f1= new TFile(oss.str().c_str());

				if (!f1->IsZombie()) {
					TTree *FastAnalysis = (TTree*)f1->Get("FastAnalysis");
					//t_fast fast;
					//FastAnalysis->SetBranchAddress("FastAnalysis",&(fast.fNbOM));
					unsigned fNbDetected[4][100]; //[limit][boh]
					FastAnalysis->SetBranchAddress("Detection",fNbDetected);
					FastAnalysis->GetEntry(0);
					unsigned aa1 = 0, aa2 = 0, aa3 = 0;
					for (int ii = 0; ii < 100; ii++) {
						//aa2 += fNbDetected[0][ii]+fNbDetected[1][ii]+fNbDetected[2][ii];
						aa2 += fNbDetected[1][ii]+fNbDetected[2][ii];
						//aa3 += fNbDetected[2][ii];
						if (aa2max < aa2) aa2max = aa2;

					}
					cout << aa2*scale << "\t";

				} else {
					cout << "file not found\t";
				}
				f1->Close();
				delete f1;

			}
		}
		cout << "\t" << endl;
	}
	
}

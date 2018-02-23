struct t_fast{
    ULong64_t fNbOM=0;
    ULong64_t fNbPhoto=0;
    unsigned fNbDetected[3][100]; //[limit][boh]
};
void makeAAfulltable(){
	cout << "AA 2D table." << endl;
	cout << "First row - fixed energies (eV)    at which the simulation is done" << endl;
	cout << "Second row - fixed wavelengths (nm) at which the simulation is done" << endl;
	cout << "Third row - AA values (cm^2) for 0 deg" << endl;
	cout << "Forth row - AA values (cm^2) for 1 deg and so on (step 1 deg till 180)" << endl;

	double hc = 197.327;  //MeV*fm 1e6*1e-6  [ev*nm]
	double e1 = 1.7;
	double e2 = 4.4;
	double step = (e2-e1)/27.;
	TGraphErrors *gAA1 = new TGraphErrors();
	TGraphErrors *gAA2 = new TGraphErrors();
	TGraphErrors *gAA3 = new TGraphErrors();
	double aa2max = 0;
	double angle;
	double scale = TMath::Pi()*21.59*21.59/1e6;  //beam radius per number of generated photons per each scan angle

	for (angle = -2; angle <= 180; angle+=1) {
		for (double e = e1; e <= e2; e+=step) {
			if (angle == -2) {
				cout << e << " ";
			} else if (int(angle) == -1) {
				double lambda=2*TMath::Pi()*hc/e;
				cout << lambda << " ";
			} else {
				ostringstream oss;
				oss << "../../prod/AAfull_ANTARES_" << angle << "deg-" << setprecision(1) << fixed << e << "eV" << ".root";
				//cout << oss.str().c_str() << endl;
				//continue;
				TFile *f1 = new TFile(oss.str().c_str());
				if (!f1->IsZombie()) {
					//TTree *FastAnalysis = (TTree*)f1->Get("FastAnalysis");
					TTree *Hit = (TTree*)f1->Get("Hit");
					unsigned aa1 = 0, aa2 = 0, aa3 = 0;
					//aa1 += fNbDetected[0][ii]+fNbDetected[1][ii]+fNbDetected[2][ii];
					aa2 += Hit->GetEntries("PMID == 23 && (Limit == 2 || Limit == 3)");
					//aa2 += Hit->GetEntries("PMID == 23 && (Limit == 1 || Limit == 2 || Limit == 3)");
					//aa3 += fNbDetected[2][ii];
					if (aa2max < aa2) aa2max = aa2;
					cout << aa2*scale << " " << flush; //<< "+-" << sqrt(aa2)*scale << " " << flush;

				} else {
					cout << "file not found ";
					return;
				}
				f1->Close();
				delete f1;

			}
		}
		cout << " " << endl;
	}
	

}

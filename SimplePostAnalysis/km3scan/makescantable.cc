#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "TMath.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TTree.h"
using namespace std;

struct t_fast{
    ULong64_t fNbOM=0;
    ULong64_t fNbPhoto=0;
    unsigned fNbDetected[3][100]; //[limit][boh]
};
void makescantable(){

	//"reflex", "noreflex" - electron production 1
	// "adjusted2"         - dynode structure, electron production 3
	//string prodnames[] = {"reflex", "noreflex", "adjusted1", "adjusted2"};
	//threestep_noreflex - there is transmission bug
	//string prodnames[] =  {"threestep_air_pol0.05_dynods0.25_cap0.45_1e6","tuned","detectionbugfix", "detectionbugfix_nottuned", "detectionbugfix_tuned"};//, "threestep_air_pol0.01", "threestep_air_pol0.05", "threestep_air_pol0.15","threestep_air_pol0.05_dynods0.3", "threestep_air_pol0.05_dynods0.25_cap_0.4","threestep_air_pol0.05_dynods0.25_cap0.45"};
	string prodnames[] =  {"detectionbugfix_nottuned", "detectionbugfix_tuned", "trunknew"};
	//string prodtitles[] = {"tuned dynode and glass surfaces","tuned collection efficiency","detection bug fixing", "detection bug fixing not tuned", "detection bug fixed, tuned"}; //"PMT glass polishing 0.01", "PMT glass polishing 0.05", "PMT glass polishing 0.15", "PMT glass polishing 0.05, dynods stack reflectivity 0.3-0.35", "PMT glass polishing 0.05, dynods stack reflectivity 0.25, dynods cap reflectivity 0.4-0.46", "PMT glass polishing 0.05, dynods stack reflectivity 0.25, dynods cap reflectivity 0.45-0.51"};
	string prodtitles[] =  {"detection bug fixing not tuned", "detection bug fixed, tuned", "current trunk"};
	TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);

	ostringstream oss;
	TGraphErrors *gAA2;
	for (int iprod = 0; iprod < 3; iprod++) {
		gAA2 = new TGraphErrors();
		oss.str("");
		oss << "g" << prodnames[iprod];
		gAA2->SetName(oss.str().c_str());
		gAA2->SetMarkerColor(iprod+2);
		gAA2->SetMarkerStyle(20);
		gAA2->SetMarkerSize(1);
		gAA2->SetTitle(";scan x (mm); Efficiency (%)");

		double aa2max = 0;
		double shift;
		double scale = 1e-6;
		if (iprod < 2) scale = 1/1e6;  //
		//else if (iprod >= 3) scale = 1/1e6;//
		//else scale = 1/1e4;


		for (shift = 0; shift <= 45; shift+=1) {
					ostringstream oss;
					oss << "prod/" << prodnames[iprod] << "/KM3scan_" << shift << "mm.root";

					//cout << oss.str().c_str() << endl;
					TFile *f1 = new TFile(oss.str().c_str());
					if (!f1->IsZombie()) {
						TTree *FastAnalysis = (TTree*)f1->Get("FastAnalysis");
						//t_fast fast;
						//FastAnalysis->SetBranchAddress("FastAnalysis",&(fast.fNbOM));
						unsigned fNbDetected[4][100]; //[limit][boh]
						FastAnalysis->SetBranchAddress("Detection",fNbDetected);
						FastAnalysis->GetEntry(0);
						unsigned aa1 = 0, aa2 = 0, aa3 = 0;
						for (int ii = 0; ii < 100; ii++) {
							//aa1 += fNbDetected[0][ii]+fNbDetected[1][ii]+fNbDetected[2][ii];
							aa2 += fNbDetected[1][ii]+fNbDetected[2][ii];
							//aa3 += fNbDetected[2][ii];
							if (aa2max < aa2) aa2max = aa2;

						}
						cout << aa2*scale << endl;
						gAA2->SetPoint(gAA2->GetN(),shift, aa2*scale);

					} else {
						cout << "file not found\t";
					}
					f1->Close();
					delete f1;

		}
		if (iprod == 0) gAA2->Draw("AP");
		else gAA2->Draw("P");
		leg->AddEntry(gAA2,prodtitles[iprod].c_str(),"p");
	}

	
	double x,y;
	TGraph *gQEmeas_pos = new TGraph();
	gQEmeas_pos->SetName("gQEmeas_pos");
        TGraph *gQEmeas_neg = new TGraph();
	gQEmeas_neg->SetName("gQEmeas_neg");

	ifstream ifs("KM3NeT_scan_QE.csv");
	while (ifs >> x >> y) {
		if (x>=0) gQEmeas_pos->SetPoint(gQEmeas_pos->GetN(), x, y);
		if (x<=0) gQEmeas_neg->SetPoint(gQEmeas_neg->GetN(), -x, y);
	}
	gQEmeas_pos->SetMarkerStyle(20);
	gQEmeas_pos->SetMarkerColor(1);
	gQEmeas_pos->Draw("P");
	leg->AddEntry(gQEmeas_pos,"QE scan (positive direction)","p");

        gQEmeas_neg->SetMarkerStyle(24);
        gQEmeas_neg->SetMarkerColor(1);
        gQEmeas_neg->Draw("P");
	leg->AddEntry(gQEmeas_neg,"QE scan (negative direction)","p");




	ifs.close();
	ifs.clear();
	ifs.seekg(0, ios::beg);
	ifs.open("KM3NeT_scan_DE.csv");
        TGraph *gDEmeas_pos = new TGraph();
        gDEmeas_pos->SetName("gDEmeas_pos");
        TGraph *gDEmeas_neg = new TGraph();
        gDEmeas_neg->SetName("gDEmeas_neg");

        while (ifs >> x >> y) {
		cout << x << " " << y << endl;
                if (x>=0) gDEmeas_pos->SetPoint(gDEmeas_pos->GetN(), x, y);
                if (x<=0) gDEmeas_neg->SetPoint(gDEmeas_neg->GetN(), -x, y);
        }
        gDEmeas_pos->SetMarkerStyle(20);
        gDEmeas_pos->SetMarkerColor(4);
        gDEmeas_pos->Draw("P");
	leg->AddEntry(gDEmeas_pos,"DE scan (positive direction)","p");

        gDEmeas_neg->SetMarkerStyle(24);
        gDEmeas_neg->SetMarkerColor(4);
        gDEmeas_neg->Draw("P");
	leg->AddEntry(gDEmeas_neg,"DE scan (negative direction)","p");
	leg->Draw();

//ANGULAR EFFICIENCY CREATION
	//in the new OMGsim version PhotoT is not saved (this info is not updated since Stepping action is related to the e-)...
	//so the conversion table created manually running scan with nm width bin and printing theta
	ifstream ifs_scanx_theta("scanx_theta.txt");
	double angle_theta, scanx;
	TGraph *gtheta2scanx = new TGraph();
	while (ifs_scanx_theta >> scanx >> angle_theta)  gtheta2scanx->SetPoint(gtheta2scanx->GetN(),angle_theta,scanx);
	//new TCanvas();
	//gtheta2scanx->Draw("AP");
	gtheta2scanx->SetMarkerStyle(20);

	cout << "ANGULAR_EFFICIENCY" << endl;
	for (double angle = 0; angle <= 90; angle+=1) {
		double angle_theta = angle/180.*TMath::Pi()/3.*2.1;
		if (angle_theta<=gtheta2scanx->GetX()[gtheta2scanx->GetN()-1]) {
			double scanx = gtheta2scanx->Eval(angle_theta, 0, "S");
			double measDE;
			if (scanx == 0) measDE = (0.191188+0.194693)/2.; //average by hand between two central points
			else measDE = (gDEmeas_pos->Eval(scanx, 0, "S")+gDEmeas_neg->Eval(scanx, 0, "S"))/2.;
			//cout << scanx << " ";
			cout << angle_theta << " " << measDE/gAA2->Eval(scanx, 0, "S") << endl;  //the last sim graph is used here!
		}
	}

}

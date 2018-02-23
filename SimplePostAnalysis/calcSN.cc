void calcSN(){
	//sqrt(NBlocks * NDOMs * NLines * T_SN)
	double scaledet;
	//to scale muon flux (1 for ORCA, 1/4 for ARCA)
	double muonscale;
	string detname[] = {"ARCA", "ORCA", "ARCA2DU", "HOURS"};

	cout << "SNtag\tCoincLvl\tNsign\tNk40\tNmu\tS@10kpc\tD_3sigma\tD_5sigma\tD_1fpw\tst.err.(%)\n";
	TCanvas *c_rates = new TCanvas();
	c_rates->SetLogy();
	
	for (int idet = 0; idet < 4; idet++) {
		if (detname[idet] == "ARCA") {
			scaledet = sqrt(2*115*18*0.1); //ARCA
			muonscale = 1./4.;   //ARCA
		} else if (detname[idet] == "ORCA") {
			scaledet = sqrt(1*115*18*0.1); //ORCA
			muonscale = 1;   //ORCA
		} else if (detname[idet] == "ARCA2DU") {
			scaledet = sqrt(2*18*0.1); //2 Lines (current situation)
			muonscale = 1./4.;   //ARCA
		} else if (detname[idet] == "HOURS") {
						scaledet = sqrt(6160*0.1); //2 Lines (current situation)
						muonscale = 1.;   //ORCA like
		}
		cout << "detector: " << detname[idet] << endl;

		//histograms produced by NoiseCoincidences have (N/Ngen)*Vgen in each bin, so *Activity is needed to have rates

		double scalek40 = 13750; //in (Bq/m3)

		// 1 kton = 1000 ton = (1000/1.03) m^3   (rho - 1030 kg/m^3 = 1.03 ton/m^3)
		// "Activity" = npos / (1000/1.03) / 0.1 s
		double npos[] = {45.7, 53.6, 61.1, 65.7};  //number of positrons old, 12 MeV, 14 MeV, 16 MeV
		double scalesn = 1./(1000/1.03) / 0.1; //in (Bq/m3) per 1 positron *npos is missing
		
		string SNtag[] = {"SN12MeV", "SN14MeV", "SN16MeV", "SNold"};

		ostringstream oss;
		oss.str("");
		oss << "coinc_K40.root";
		TFile *fK40 = new TFile(oss.str().c_str());
		TH1F *hcoincK40 = (TH1F *)fK40->Get("hcoincidences");
		oss.str("");
		oss << "k40";
		hcoincK40->SetName(oss.str().c_str());
		hcoincK40->Scale(scalek40);  //k40 bq = 13750 m-3 sec-1
		hcoincK40->SetLineWidth(2);
		hcoincK40->SetMarkerStyle(20);
		hcoincK40->SetTitle("Rates; coincidence level; rate [Hz]");
		if (idet == 0) hcoincK40->Draw();

		TH1F *hmuons = new TH1F("muons","",30,0,30);
		hmuons->Sumw2();
		TH1F *hmuons_orca = new TH1F("muons_orca","",30,0,30);
		ifstream ifs("muons_from_ppm_du.csv");
		int x;
		double y;
		while(ifs>>x>>y) {
			hmuons->SetBinContent(x+1,y);
			hmuons->SetBinError(x+1,0);

			hmuons_orca->SetBinContent(x+1,y);
			hmuons_orca->SetBinError(x+1,0);
		}

		if (idet == 0) {
			hmuons->SetMarkerStyle(25);
			hmuons->SetMarkerSize(0.8);
			hmuons->Draw("psame");
			hmuons->Scale(muonscale);
		
			hmuons_orca->SetMarkerStyle(21);
			hmuons_orca->SetMarkerSize(0.8);
			hmuons_orca->Draw("psame");
		}

		TH1F* hbg = (TH1F*)hcoincK40->Clone("bg");
		hbg->Add(hmuons);
		//hbg->Draw("same");

		int bestcoincleft, bestcoincright;
		double bestsignificance, err;

		TCanvas *c_significance = new TCanvas();
		oss.str("");
		oss << "Significance for " << detname[idet];
		c_significance->SetTitle(oss.str().c_str());


		for (int ii = 0; ii < 4; ii++) {
			oss.str("");
			oss << "coinc_" << SNtag[ii] << ".root";
			TFile *fSN = new TFile(oss.str().c_str());
			TH1F *hcoincSN = (TH1F *)fSN->Get("hcoincidences");
			hcoincSN->SetName(SNtag[ii].c_str());
			hcoincSN->Scale(scalesn*npos[ii]);
			hcoincSN->SetLineColor(ii+1);
			hcoincSN->SetLineWidth(2);
			hcoincSN->SetMarkerStyle(30);
			hcoincSN->SetMarkerSize(2);
			hcoincSN->SetMarkerColor(ii+1);
			c_rates->cd();
			if (idet == 0) hcoincSN->Draw("same");

			TGraph *gsignificance = new TGraph();
			gsignificance->SetMarkerStyle(20);
			oss.str("");
			oss << "significance_" << SNtag[ii];
			gsignificance->SetName(oss.str().c_str());
			bestsignificance = 0;
			//interval cut optimisation
			for (int kk = 0; kk < hcoincSN->GetNbinsX(); kk++){
				for (int ll = hcoincSN->GetNbinsX(); ll >= kk; ll--){
					//double sign = hcoincSN->GetBinContent(kk);
					//double bg = hbg->GetBinContent(kk);
					double sign = hcoincSN->Integral(kk,ll);
					double bg = hbg->Integral(kk,ll);
					if (bg != 0) {
						double significance = sign/sqrt(bg)*scaledet;
						gsignificance->SetPoint(gsignificance->GetN(),hcoincSN->GetBinLowEdge(kk), significance);
						if (significance > bestsignificance) {
							bestsignificance = significance;
							bestcoincleft = kk; //hcoincSN->GetBinLowEdge(kk);
							bestcoincright = ll; //hcoincSN->GetBinLowEdge(ll);
							err = sqrt( pow(hcoincSN->GetBinError(kk)/sign,2.) + pow(0.5*hbg->GetBinError(kk)/bg,2.) ); //statistical error
						}
					}
				}

			}
			c_significance->cd();
			gsignificance->SetMarkerColor(ii+1);
			gsignificance->SetTitle(";coincidence level cut; significance level");
			if (ii == 0) gsignificance->Draw("AP");
			else gsignificance->Draw("P");


			double dist_3sigma = 10.*sqrt(bestsignificance/3); //distance for 5 sigma sign. 
			double dist_5sigma = 10.*sqrt(bestsignificance/5); //distance for 5 sigma sign. 
			double dist_1fpw = 10.*sqrt(bestsignificance/(TMath::ErfInverse(1.-2./(7.*24*60*60*10))*sqrt(2))); //distance for 1 fake event per week threshold
			double interr;


			cout << SNtag[ii] << "\t" << hcoincSN->GetBinLowEdge(bestcoincleft) << "--" << hcoincSN->GetBinLowEdge(bestcoincright) << "\t" << setprecision(3) << 
			pow(scaledet,2)*hcoincSN->IntegralAndError(bestcoincleft,bestcoincright,interr) << "$\\pm$" << setprecision(1) << pow(scaledet,2)*interr << "\t" << setprecision(2) <<
			pow(scaledet,2)*hcoincK40->IntegralAndError(bestcoincleft,bestcoincright,interr) << "$\\pm$" << setprecision(1) << pow(scaledet,2)*interr << "\t" << setprecision(2) <<
			pow(scaledet,2)*hmuons->IntegralAndError(bestcoincleft,bestcoincright,interr) << "$\\pm$" << setprecision(1) << pow(scaledet,2)*interr << "\t" << setprecision(2) <<
			bestsignificance << "\t" << dist_3sigma << "\t" << dist_5sigma << "\t" << dist_1fpw << "\t" << err*100 << endl;
		}

		if (idet == 0) {
			c_rates->cd();
			hcoincK40->Draw("same");
		}
	}

}

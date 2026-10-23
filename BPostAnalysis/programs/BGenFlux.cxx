#include "BPrimaryGeneratorAction.hh"  // Adjust to your actual header
#include "TApplication.h"


#include <TF2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TStyle.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TLatex.h>
#include <TString.h>

#include <cmath>
#include <iostream>

// ------------------------------------------------------------
// Generate muons from the modified flux and plot the samples.
// Energy: GeV
// Zenith angle: radians internally, degrees in the plots.
// ------------------------------------------------------------

void SampleModifiedFlux()
{
    const int N = 1000000;

    // const double Emin = 1.0;       // GeV
    // const double Emax = 1.0e4;     // GeV
    // const double thetaMin = 0.0;
    // const double thetaMax = TMath::Pi() / 2.0;

    // Geant4 units
    const double Emin = 3e-1;
    const double Emax = 1e4;

    const double thetaMin = 0.;
    const double thetaMax = 60.0;

    TF2* samplingFlux = new TF2(
        "samplingFlux",
        [](double* x, double* pars) -> double
        {
            double u = x[0];
            const double E = pow(10, u); // is in GeV
            const double theta = x[1]*deg; // to rad

            // std::cout << "\r\033[10C" << "E: " << E 
            //                             << " - theta deg: " << theta/deg
            //                             << " - theta rad: " << theta
            //                             << std::endl;
            double xx[2] = {E, theta};

            const double flux =
                lactel::BPrimaryGeneratorAction::ModifiedFormula2D( xx, pars);

            return flux
                *  E * log10(10.0)
                * std::sin(theta)
                * std::cos(theta);
                
        },
        log10(Emin),
        log10(Emax),
        thetaMin,
        thetaMax,
        0
    );

    const double angRes = 0.5; // deg
    const double eneRes = 10; // GeV

    const int nEnergyBins = (Emax - Emin)/eneRes;
    const int nThetaBins = (thetaMax - thetaMin)/angRes;

    samplingFlux->SetNpx(nEnergyBins);
    samplingFlux->SetNpy(nThetaBins);

    // --------------------------------------------------------
    // 2. Histograms with logarithmic energy binning.
    // --------------------------------------------------------

    double energyEdges[nEnergyBins + 1];

    const double logEmin = std::log10(Emin);
    const double logEmax = std::log10(Emax);

    for (int i = 0; i <= nEnergyBins; ++i)
    {
        const double logE =
            logEmin + (logEmax - logEmin) * i / nEnergyBins;

        energyEdges[i] = logE; //std::pow(10.0, logE);
    }
    
    std::cout << "\r\033[10C" << "Energy: ";
    for(auto& ed : energyEdges)
         std::cout << ed << " "; 
    std::cout << std::endl;

    TH2D* h2 = new TH2D(
        "h2",
        "Sampled modified muon flux;"
        "Muon energy log10(E) [GeV];"
        "Zenith angle #theta [degrees]",
        nEnergyBins, energyEdges,
        nThetaBins, thetaMin, thetaMax
    );

    TH1D* hEnergy = new TH1D(
        "hEnergy",
        "Energy projection;"
        "Muon energy log10(E) [GeV];"
        "Number of sampled muons",
        nEnergyBins, energyEdges
    );

    TH1D* hTheta = new TH1D(
        "hTheta",
        "Angular projection;"
        "Number of sampled muons;"
        "Zenith angle #theta [degrees]",
        nThetaBins, thetaMin, thetaMax
    );

    // --------------------------------------------------------
    // 3. Generate random samples.
    // --------------------------------------------------------

    TRandom3 rng(12345);

    for (int i = 0; i < N; ++i)
    {
        double E;
        double theta;

        samplingFlux->GetRandom2(E, theta, &rng);

        // std::cout << "\r\033[10C" << "E: " << E/GeV 
        //                             << "E: " << theta/deg
        //                             << std::endl;

        // const double logE = std::pow(10.0, log10(E));
        const double thetaDeg = theta;

        h2->Fill(E, thetaDeg);
        hEnergy->Fill(E);
        hTheta->Fill(thetaDeg);
    }

    std::cout << "Generated " << N << " muons." << std::endl;

    // --------------------------------------------------------
    // 4. Draw the three plots on one canvas.
    // --------------------------------------------------------

    gStyle->SetOptStat(0);
    gStyle->SetNumberContours(100);

    TCanvas* c = new TCanvas(
        "c", "Modified muon flux sampling", 1400, 900
    );

    // Main 2D heatmap: upper-left.
    TPad* p1 = new TPad("p1", "Heatmap", 0.00, 0.32, 0.76, 1.00);
    p1->SetRightMargin(0.15);
    p1->SetLeftMargin(0.12);
    p1->SetBottomMargin(0.12);
    // p1->SetLogx();

    // Angular projection: upper-right.
    TPad* p2 = new TPad("p2", "Angular projection",
                        0.76, 0.32, 1.00, 1.00);
    p2->SetLeftMargin(0.20);
    p2->SetRightMargin(0.10);
    p2->SetBottomMargin(0.12);
    // p2->SetLogx();

    // Energy projection: bottom-left.
    TPad* p3 = new TPad("p3", "Energy projection",
                        0.00, 0.00, 0.76, 0.32);
    p3->SetLeftMargin(0.12);
    p3->SetRightMargin(0.15);
    p3->SetBottomMargin(0.20);
    // p3->SetLogx();
    p3->SetLogy();

    // Information panel: bottom-right.
    TPad* p4 = new TPad("p4", "Information",
                        0.76, 0.00, 1.00, 0.32);

    p1->Draw();
    p2->Draw();
    p3->Draw();
    p4->Draw();

    // ---- Main heatmap ----
    p1->cd();

    h2->GetXaxis()->SetTitle("Muon energy log10(E) [GeV]");
    h2->GetYaxis()->SetTitle("Zenith angle #theta [degrees]");
    h2->GetXaxis()->SetTitleSize(0.045);
    h2->GetYaxis()->SetTitleSize(0.045);
    h2->GetXaxis()->SetLabelSize(0.035);
    h2->GetYaxis()->SetLabelSize(0.035);
    h2->SetTitle("Sampled muon distribution");

    h2->Draw("COLZ");

    // ---- Angular projection ----
    p2->cd();

    hTheta->SetFillColor(kAzure - 9);
    hTheta->SetLineColor(kBlue + 2);
    hTheta->SetLineWidth(2);

    hTheta->GetXaxis()->SetTitle("Zenith angle [degrees]");
    hTheta->GetYaxis()->SetTitle("Sample count");
    hTheta->GetXaxis()->SetTitleSize(0.045);
    hTheta->GetYaxis()->SetTitleSize(0.045);
    hTheta->GetXaxis()->SetLabelSize(0.035);
    hTheta->GetYaxis()->SetLabelSize(0.035);

    // Horizontal bars: count on x-axis, angle on y-axis.
    hTheta->Draw("BAR");

    // ---- Energy projection ----
    p3->cd();

    hEnergy->SetFillColor(kAzure - 9);
    hEnergy->SetLineColor(kBlue + 2);
    hEnergy->SetLineWidth(2);

    hEnergy->GetXaxis()->SetTitle("Muon energy log10(E) [GeV]");
    hEnergy->GetYaxis()->SetTitle("Sample count");
    hEnergy->GetXaxis()->SetTitleSize(0.065);
    hEnergy->GetYaxis()->SetTitleSize(0.065);
    hEnergy->GetXaxis()->SetLabelSize(0.055);
    hEnergy->GetYaxis()->SetLabelSize(0.055);

    hEnergy->Draw("HIST");

    // ---- Information panel ----
    p4->cd();

    TLatex text;
    text.SetNDC();
    text.SetTextSize(0.075);

    text.DrawLatex(0.08, 0.85, "Modified muon flux");
    text.DrawLatex(0.08, 0.68, Form("Samples: %d", N));
    text.DrawLatex(0.08, 0.52, Form("E: %.0f - %.0f GeV", Emin, Emax));
    text.DrawLatex(0.08, 0.36, "#theta: 0 - 90 degrees");
    text.DrawLatex(0.08, 0.20, "Horizontal detector");
    text.DrawLatex(0.08, 0.08, "#Phi(E,#theta) sin#theta cos#theta");

    // --------------------------------------------------------
    // 5. Save the output.
    // --------------------------------------------------------

    c->SaveAs("modified_flux_sampling.png");
    c->SaveAs("modified_flux_sampling.pdf");

    // Save histograms for later analysis.
    TFile* output = new TFile("modified_flux_sampling.root", "RECREATE");

    h2->Write();
    hEnergy->Write();
    hTheta->Write();

    output->Close();

    std::cout << "Saved plots and histograms." << std::endl;
}

int main(int argc, char* argv[]){

    gStyle->SetPalette(53);
    
    TApplication app("ROOTApp", &argc, argv);

    SampleModifiedFlux();

    std::cout << "\r\033[10C" << "Waiting for user action" << std::endl;
    app.Run();
}
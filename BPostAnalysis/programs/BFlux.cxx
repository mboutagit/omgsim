
#include <iostream>
#include "TApplication.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TF2.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TStyle.h"

#include "BPrimaryGeneratorAction.hh"
#include "BGenerator.hh"


using namespace std;
using namespace lactel;

int main(int argc, char* argv[]){
    TApplication app("ROOTApp", &argc, argv);

    gStyle->SetOptStat(0);

    const Int_t nColors = 255;
    // Define stops (from low to high)
    Double_t stops[3] = { 0.00, 0.50, 1.00 };

    // RGB values for a dark → orange → light orange gradient
    Double_t red[3]   = { 0.10, 1.00, 1.00 };
    Double_t green[3] = { 0.05, 0.40, 0.85 };
    Double_t blue[3]  = { 0.00, 0.00, 0.10 };

    Int_t palette[nColors];

    Int_t fi = TColor::CreateGradientColorTable(
        3, stops, red, green, blue,
        nColors, 1.0
    );

    for (int i = 0; i < nColors; i++) {
        palette[i] = fi + i;
    }

    gStyle->SetPalette(nColors, palette);
    gStyle->SetNumberContours(nColors);



    TCanvas* can = new TCanvas("can", "Muon Flux", 1700, 1000);
    can->Divide(2,2);
    can->cd(1);
    gPad->SetLogx();
    gPad->SetLogy();
    gPad->SetGrid();

    int angles[] = {0, 30, 60, 75, 90}; // angles in degree

    double style = 0;
    TLegend* leg = new TLegend(0.5, 0.1, 0.7, 0.5);
    for(auto& angle : angles){
        TF1* flux = new TF1("flux_" + (int)angle, [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::gaisser_flux(x, pars);}, 0.1, 1e4, 1);
        TF1* modFlux = new TF1("modFlux_"+ (int)angle, 
                                [](double* x, double* pars){ return pow(x[0], 2.7) *
                                    lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 
                                0.1, 1e4, 1
                            );
        flux->SetParameter(0, angle);
        modFlux->SetParameter(0, angle);

        flux->SetLineColor(style+1);
        modFlux->SetLineColor(style+1);

        flux->SetLineStyle(7);
        // modFlux->SetLineStyle(7);

        if(style == 0){
            flux->SetTitle("muons flux; E_{#mu} (GeV); E_{#mu}^{2.7} * I_{#mu} (GeV^{1.7}cm^{-2}S^{-1}Sr^{-1})");
            modFlux->SetTitle("muons flux; E_{#mu} (GeV); E_{#mu}^{2.7} * I_{#mu} (GeV^{1.7}cm^{-2}S^{-1}Sr^{-1})");
            // flux->Draw();
            modFlux->Draw("");
            // leg->AddEntry(flux, ("Gaiser - " + std::to_string(angle) + "^{°}").c_str(), "l");
            leg->AddEntry(modFlux, ("#theta= " + std::to_string(angle) + "^{°}").c_str(), "l");
        }
        else{
            // flux->Draw("same");
            modFlux->Draw("same");
            // leg->AddEntry(flux, ("Gaiser - " + std::to_string(angle) + "^{°}").c_str(), "l");
            leg->AddEntry(modFlux, ("#theta= " + std::to_string(angle) + "^{°}").c_str(), "l");
        }

        style++;
    }
    leg->Draw("same");

    // // Grieder book 

    can->cd(2);
    gPad->SetLogx();
    gPad->SetLogy();
    gPad->SetGrid();
    TLegend* leg1 = new TLegend(0.7, 0.7, 0.85, 0.85);

    int angle = 0;
    TF1* modFlux = new TF1("modFlux_0", [](double* x, double* pars){ return pow(x[0], 2.7) * 1/1e3 * lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 1e-3, 1e5, 1);
    TF1* modFlux30 = new TF1("modFlux_30", [](double* x, double* pars){ return pow(x[0], 2.7) * 1/1e3 * lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 1e-3, 1e5, 1);
    TF1* modFlux60 = new TF1("modFlux_60", [](double* x, double* pars){ return pow(x[0], 2.7) * 1/1e3 * lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 1e-3, 1e5, 1);
    TF1* modFlux90 = new TF1("modFlux_90", [](double* x, double* pars){ return pow(x[0], 2.7) * 1/1e3 * lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 1e-3, 1e5, 1);
    modFlux->SetParameter(0, 0);
    modFlux30->SetParameter(0, 30);
    modFlux60->SetParameter(0, 60);
    modFlux90->SetParameter(0, 90);
    modFlux->SetLineColor(2);
    modFlux30->SetLineColor(3);
    modFlux60->SetLineColor(4);
    modFlux90->SetLineColor(6);
    modFlux->Draw();
    modFlux30->Draw("same");
    modFlux60->Draw("same");
    modFlux90->Draw("same");
    leg1->AddEntry(modFlux, ("Modified - " + std::to_string(0) + "^{°}").c_str(), "l");
    leg1->AddEntry(modFlux30, ("Modified - " + std::to_string(30) + "^{°}").c_str(), "l");
    leg1->AddEntry(modFlux60, ("Modified - " + std::to_string(60) + "^{°}").c_str(), "l");
    leg1->AddEntry(modFlux90, ("Modified - " + std::to_string(90) + "^{°}").c_str(), "l");

    modFlux->SetTitle("muons flux; E_{#mu} (GeV); E_{#mu}^{2.7} * I_{#mu} (MeV^{-1}cm^{-2}S^{-1}Sr^{-1})");


    pair<double, double> data[] = { {0.2827795772525707, 0.0000017378008287493763}, // data extarcted from Grieder book using https://automeris.io/ / Already in MeV MeV^{-1}cm^{-2}S^{-1}Sr^{-1}
                                    {0.6196743835946624, 0.0000033113112148259145},
                                    {0.39468637057618516, 0.000002754228703338169},
                                    {0.9729151303675503, 0.0000032111949093648306},
                                    {2.050003888503319, 0.0000018478497974222944},
                                    {4.581293070324709, 6.118805757518225e-7},
                                    {14.289780363661004, 6.30957344480193e-8},
                                    {29.524829173516423, 1e-8},
                                    {81.86863451230586, 6.309573444801942e-10},
                                    {214.03955406243756, 3.9810717055349857e-11},
                                    {459.92990903830207, 3.1141055844467772e-12},
                                    {1202.4530910643005, 1.0311772745930601e-13}
                                    };

    TGraph* gr = new TGraph();
    int j=0;
    for(auto& val : data){
        gr->SetPoint(j, val.first, pow(val.first, 2.7) * val.second);
        j++;
    }
    gr->SetMarkerColor(6);
    gr->SetMarkerStyle(20);
    
    leg1->AddEntry(gr, "Measurements at 0°", "P");

    gr->Draw("samep");
    leg1->Draw();

    can->cd(3);

    double Emin = 1e-3;
    double Emax = 1e5;
    double Thetamin = 0.;
    double Thetamax = TMath::Pi()/2;


    TF2* modFlux2D = new TF2("modFlux2D", 
                                    [](double* x, double* pars){
                                        return 
                                        lactel::BPrimaryGeneratorAction::ModifiedFormula2D(x, pars);
                                    },
                                    Emin, Emax, Thetamin, Thetamax
                                );

    double fixedAngle = 60;
    

    
    gPad->SetLogx();
    modFlux2D->Draw("colz");
    double disMax = modFlux2D->GetMaximum();
    std::cout << "\r\033[10C" << "Dist. Max: " << disMax << std::endl;

    can->cd(4);

    TH2D* dist = new TH2D("dist", "test Modified 2D sampling; Log_{10}(Energy) [GeV]; Angle [deg]",
            500, Emin, Emax,
            500, Thetamin, Thetamax
        );

    TH1D* ene = new TH1D("ene", "Energy; Eneregy in GeV; Count", 100, Emin, Emax);
    TH1D* theta = new TH1D("theta", "Theta; Theta in deg; Count", 100, Thetamin, Thetamax);

    TH3D* dist3D = new TH3D("dist3D", "test Modified 2D sampling; Log_{10}(Energy) [GeV]; Angle [deg]; Intensity ",
            100, Emin, Emax,
            100, Thetamin, Thetamax,
            100, 0, 1
        );

    TH1D* distcheck = new TH1D("distcheck", "Eval check; Events ",
        100, -5, 2
    );
    

    for(int i = 0; i < 10e6; i++){
        double x, y;
        modFlux2D->GetRandom2(x, y);
        dist->Fill(x, y);
        dist3D->Fill(x, y, modFlux2D->Eval(x, y)/disMax);
        distcheck->Fill(log10(modFlux2D->Eval(x, 0)/disMax));
        ene->Fill(x);
        theta->Fill(y);

    }

    gPad->SetLogx();
    dist->Draw("colz");

    new TCanvas();
    ene->Draw();
    new TCanvas();
    theta->Draw();

    new TCanvas();
    gPad->SetLogz();
    gPad->SetLogx();
    dist3D->Draw();

    new TCanvas();
    gPad->SetLogx();
    distcheck->Draw();

    new TCanvas();
    gPad->SetLogx();
    gPad->SetLogy();
    bool firstDraw = true;
    for(int i = 0; i < 90; i++){
        double ang = i * rad;

        TF1* test60 = new TF1("modFlux_70", [modFlux2D, ang](double* x, double* pars)
                                { return modFlux2D->Eval(x[0], ang); }, 
                                1e-3, 1e5, 0
                            );
        test60->SetLineColor(i);
        if(firstDraw){
            test60->Draw();
            firstDraw = false;
        }
        else{
            test60->Draw("same");
        }

    }

    new TCanvas();
    gPad->SetLogy();
    bool firstDrawEne = true;
    for(int i = 1; i < 30; i++){
        double ene =  i*100/1e3;

        TF1* testEne = new TF1("Ene", [modFlux2D, ene](double* x, double* pars)
                                { return modFlux2D->Eval(ene, x[0]); }, 
                                0, 90, 0
                            );
        // testEne->SetTitle(("Energy: " + to_string(ene) + "; Energy [MeV]; Abundance").c_str());               
        testEne->SetLineColor(i);
        if(firstDrawEne){
            testEne->Draw();
            firstDrawEne = false;
        }
        else{
            testEne->Draw("same");
        }

    }


    new TCanvas();
    gPad->SetLogy();
    TGraph grInt;
    TGraph grInt1;
    TGraph grInt2;
    for(int i = 0; i < 90; i++){
        double ang = i * rad;

        TF1* test60 = new TF1("test", [modFlux2D, ang](double* x, double* pars)
                                { return modFlux2D->Eval(x[0], ang); }, 
                                1e-3, 1e5, 0
                            );
        test60->SetLineColor(i);
        grInt.SetPoint(i, pow(cos(ang*deg), 2), test60->Integral(1e-3, 1e5));
        // grInt1.SetPoint(i, ang, test60->Integral(1e-3, 1e2));
        // grInt2.SetPoint(i, ang, test60->Integral(1e-3, 1e1));

    }

    grInt.Draw("APC");
    // grInt1.SetLineColor(2);
    // grInt1.Draw("PC");
    // grInt2.SetLineColor(3);
    // grInt2.Draw("PC");


    //#############################################################################################""



    TCanvas* can0 = new TCanvas("can0", "Muon Flux", 1700, 1000);
    can0->Divide(2,2);
    can0->cd(1);
    gPad->SetLogx();
    gPad->SetLogy();
    gPad->SetGrid();

    TF1* fun = new TF1("fun", 
                            [](double* x, double* pars)
                            { return utils::BGenerator::ModifiedGaisser(x, pars);},
                            0.001, 1e4, 0
                        );
    style = 0;
    TLegend* leg2 = new TLegend(0.5, 0.1, 0.7, 0.5);
    for(auto& angle : angles){
        TF1* flux = new TF1("flux_" + (int)angle, [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::gaisser_flux(x, pars);}, 0.1, 1e4, 1);
        
        TF1* modFlux = new TF1("modFlux_"+ (int)angle, 
                                [fun, angle](double* x, double* pars){ return pow(x[0], 2.7) * fun->Eval(x[0], angle);}, 
                                0.001, 1e4, 0
                            );

        flux->SetParameter(0, angle);
        modFlux->SetParameter(0, angle);

        flux->SetLineColor(style+1);
        modFlux->SetLineColor(style+1);

        flux->SetLineStyle(7);
        // modFlux->SetLineStyle(7);

        if(style == 0){
            flux->SetTitle("muons flux; E_{#mu} (GeV); E_{#mu}^{2.7} * I_{#mu} (GeV^{1.7}cm^{-2}S^{-1}Sr^{-1})");
            flux->Draw();
            modFlux->Draw("same");
            leg2->AddEntry(flux, ("Gaiser - " + std::to_string(angle) + "^{°}").c_str(), "l");
            leg2->AddEntry(modFlux, ("Modified - " + std::to_string(angle) + "^{°}").c_str(), "l");
        }
        else{
            flux->Draw("same");
            modFlux->Draw("same");
            leg2->AddEntry(flux, ("Gaiser - " + std::to_string(angle) + "^{°}").c_str(), "l");
            leg2->AddEntry(modFlux, ("Modified - " + std::to_string(angle) + "^{°}").c_str(), "l");
        }

        style++;
    }
    leg2->Draw("same");




    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();
    return 0;
}
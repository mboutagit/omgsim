
#include <iostream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLegend.h"

#include "BPrimaryGeneratorAction.hh"


using namespace std;
using namespace lactel;

int main(int argc, char* argv[]){
    TApplication app("ROOTApp", &argc, argv);


    TCanvas* can = new TCanvas("can", "Analy");
    can->SetLogx();
    can->SetLogy();
    can->SetGrid();


    int angles[] = {0, 30, 60, 75, 90}; // angles in degree

    double style = 0;
    TLegend* leg = new TLegend(0.5, 0.1, 0.7, 0.5);
    for(auto& angle : angles){
        TF1* flux = new TF1("flux_" + (int)angle, [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::gaisser_flux(x, pars);}, 0.1, 1e4, 1);
        TF1* modFlux = new TF1("modFlux_"+ (int)angle, [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 0.1, 1e4, 1);
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
            leg->AddEntry(flux, ("Gaiser - " + std::to_string(angle) + "^{°}").c_str(), "l");
            leg->AddEntry(modFlux, ("Modified - " + std::to_string(angle) + "^{°}").c_str(), "l");
        }
        else{
            flux->Draw("same");
            modFlux->Draw("same");
            leg->AddEntry(flux, ("Gaiser - " + std::to_string(angle) + "^{°}").c_str(), "l");
            leg->AddEntry(modFlux, ("Modified - " + std::to_string(angle) + "^{°}").c_str(), "l");
        }

        style++;
    }
    leg->Draw("same");

    // // Grieder book 

    TCanvas* can1 = new TCanvas("can1", "Analy");
    can1->SetLogx();
    can1->SetLogy();
    can1->SetGrid();
    TLegend* leg1 = new TLegend(0.7, 0.7, 0.85, 0.85);

    int angle = 0;
    TF1* modFlux = new TF1("modFlux_0", [](double* x, double* pars){ return 1/1e3 * lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 0.1, 1e3, 1);
    modFlux->SetParameter(0, 0);
    modFlux->SetLineColor(2);
    modFlux->Draw();
    leg1->AddEntry(modFlux, ("Modified - " + std::to_string(angle) + "^{°}").c_str(), "l");

    modFlux->SetTitle("muons flux; E_{#mu} (GeV); I_{#mu} (MeV^{-1}cm^{-2}S^{-1}Sr^{-1})");


    pair<double, double> data[] = { {0.2827795772525707, 0.0000017378008287493763}, // data extarcted from Grieder book using https://automeris.io/
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
        gr->SetPoint(j, val.first, val.second);
        j++;
    }
    gr->SetMarkerColor(6);
    gr->SetMarkerStyle(20);
    
    leg1->AddEntry(gr, "Grieder", "l");

    gr->Draw("samep");
    leg1->Draw();

    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();
    return 0;
}
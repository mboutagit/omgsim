
// calculate amt. muon flux according to analytical formula
// total vertical flux : 70 m^-2 s^-1 sr^-1
// after angular integration : 147 m^-2 s^-1
// .L muflux.C
// muflux()

#include "TH1D.h"
#include "TCanvas.h"
#include "TF2.h"

namespace Jurgen
{
    double Flux(double *xx, double *par) {
        double E  = xx[0];
        double ct = xx[1];
        return pow(E,-2.7)*ct*ct;
    }
    double LgFlux(double *xx, double *par) {
        double E  = pow(10.,xx[0]);
        double ct = xx[1];
        return pow(E,-2.7)*ct*ct;
    }
    double LgFlux1D(double *xx, double *par) {
        double E  = pow(10.,xx[0]);
        double ct = par[0];
        return pow(E,-2.7)*ct*ct;
    }

    TF2 getFluxFunction(float Emin, float Emax, float cosmin, float cosmax){
        int    NEbin  = 990;
        int    Ncbin  = 95;

        TF2 f2Flux("f2Flux; E_{#mu} [GeV]; cos(#theta) [rad]",Flux,Emin,Emax,cosmin,cosmax);
        f2Flux.SetNpx(NEbin);
        f2Flux.SetNpy(Ncbin);
        f2Flux.GetXaxis()->SetTitle("E_{#mu} [GeV]");
        f2Flux.GetYaxis()->SetTitle("cos(#theta) [rad]");

        return f2Flux;
    }

    void muflux(){
        double Emin   = 1.;
        double Emax   = 100.;
        int    NEbin  = 990;
        double cosmin = 0.05;
        double cosmax = 1.00;
        int    Ncbin  = 95;
    /*
        TH2D h2("h2","Muflux",NEbin,Emin,Emax,Ncbin,cosmin,cosmax);
        h2.SetMaximum(1.);
        h2.SetMinimum(0.);
        h2.SetXTitle("E_{#mu}/ GeV");
        h2.SetYTitle("cos(#theta)");
        h2.SetStats(kFALSE);
        //    h2.DrawCopy();
    */

    // 2-dim tests

        double LgEmin = log10(Emin);
        double LgEmax = log10(Emax);
        int    NLgEbin = 200;
        TH2D h2Lg("h2Lg","MuLgflux",NLgEbin,LgEmin,LgEmax,Ncbin,cosmin,cosmax);
        h2Lg.SetMaximum(1.);
        h2Lg.SetMinimum(1.e-9);
        h2Lg.SetXTitle("lg(E_{#mu}/ GeV)");
        h2Lg.SetYTitle("cos(#theta)");
        h2Lg.SetStats(kFALSE);
    //    h2Lg.DrawCopy();

        TF2 f2LgFlux("f2LgFlux",LgFlux,LgEmin,LgEmax,cosmin,cosmax);
        f2LgFlux.SetNpx(NLgEbin);
        f2LgFlux.SetNpy(Ncbin);
    //    f2LgFlux.DrawCopy("same colz");

        TF2 f2Flux("f2Flux; E_{#mu} [GeV]; cos(#theta) [rad]",Flux,Emin,Emax,cosmin,cosmax);
        f2Flux.SetNpx(NEbin);
        f2Flux.SetNpy(Ncbin);
        f2Flux.GetXaxis()->SetTitle("E_{#mu} [GeV]");
        f2Flux.GetYaxis()->SetTitle("cos(#theta) [rad]");
    //    f2Flux.DrawCopy("same colz");


    // 1-dim tests
        
        TH1D h1LgE("h1LgE","fluxE",NLgEbin,LgEmin,LgEmax);
        h1LgE.SetXTitle("lg(E_{#mu}/ GeV)");
        h1LgE.SetStats(kFALSE);

        TH1D h1E("h1E","fluxE",NEbin,Emin,Emax);
        h1E.SetXTitle("E_{#mu}/ GeV");
        h1E.SetStats(kFALSE);

        TH1D h1ct("h1ct","fluxct",Ncbin,cosmin,cosmax);
        h1ct.SetXTitle("cos(zenith)");
        h1ct.SetLineColor(2);
        h1ct.SetStats(kFALSE);
            
        int Nevent = 10000000;
        for (int i=0;i<Nevent;i++) {
            double E;
            double cost;
            f2Flux.GetRandom2(E,cost);
            h1E.Fill(E);
            h1LgE.Fill(log10(E));
            h1ct.Fill(cost);
        }
        h1E.Scale(1./Nevent);
        h1LgE.Scale(1./Nevent);
        h1ct.Scale(1./Nevent);

        TCanvas* canvas = new TCanvas("canvas", "canvas");
        canvas->Divide(2,2);

        canvas->cd(1);
        h1E.DrawCopy();
        canvas->cd(2);
        h1LgE.DrawCopy();
        canvas->cd(3);
        h1ct.DrawCopy();
        canvas->cd(4);
        f2Flux.DrawCopy("colz");
        cout << "Drawing ... " << endl;
        
        
        gPad->SetLogz();
        gPad->SetLogy();

    }

} // namespace Jurgen

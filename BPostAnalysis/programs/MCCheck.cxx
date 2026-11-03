#include "BPostAnalysis.hh"
#include "utils.hh"
#include "BPrimaryGeneratorAction.hh"
#include "TF2.h"
#include "BGenerator.hh"
#include "BLactelDetector.hh"

#include <CLI/CLI.hpp>
#include <sys/resource.h>

using namespace km3net;
using namespace lactel;
using namespace UTILS;
using namespace utils;


int main(int argc, char* argv[]){

   if(argc < 2){
    cout << "Syntax error: expected root file name" << endl;
    cout << "Syntax: " << argv[0] << " file.root" << endl;
    return 1;
  }

 

  std::string input_file;

  CLI::App cmdApp{"Command-line parser"};

  cmdApp.add_option("-f,--input-file", input_file, "input_file containing DetectedEvent")->required(); // directory containing test files

  CLI11_PARSE(cmdApp, argc, argv);

  TApplication app("MCCheck", &argc, argv);

  string rootfilename =   input_file; //app.InputFiles()->At(0)->GetName(); //"../../default.root";
  TFile* file = TFile::Open(rootfilename.c_str(), "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << rootfilename << std::endl;
        return 1;
    }
    else{
        std::cout << "File " << rootfilename << " is open" << std::endl;
    }

    TTree* genTree = (TTree*)file->Get("Gen");
    TTree* detEvtTree = (TTree*)file->Get("physicsEvent");

    float radius = 4.83; // in meters    
    float gen_surface_m2  = TMath::Pi() * radius*radius; // pi*R² disk
    float gen_surface_cm2 = gen_surface_m2 * 1e4;
 
    TF2* flux = new TF2("flux", lactel::BPrimaryGeneratorAction::ModifiedFormula2D, 1e-2, 1e5, 0, 60);
    float expected_muons_per_m2_per_s = 126; // flux->Integral(3e-1, 1e4, 0, 60); //generator->Integral(1e-1, 1e3, 0, TMath::Pi()/2); // by m²
    float expected_muons_per_cm2_per_s = expected_muons_per_m2_per_s/1e4; // to cm²
    float expected_muons_per_gensurf = expected_muons_per_cm2_per_s * gen_surface_cm2;
    float number_of_generated_muons_per_gensurf = genTree->GetEntries(); // 100 000 events
    float exposure_time = number_of_generated_muons_per_gensurf / expected_muons_per_gensurf;

    std::cout << "\r\033[10C" << "Generated Entries: " << number_of_generated_muons_per_gensurf << std::endl;

    std::cout 
    << string (100, '=')
    << "\n"
    << setw(15) << "A(m\u2082)"
    << setw(15) << "\u03BCExp"
    << setw(15) << "\u03BCGen"
    << setw(15) << "tExpo"
    <<"\n"
    << setw(15) << gen_surface_m2
    << setw(15) << expected_muons_per_m2_per_s
    << setw(15) << number_of_generated_muons_per_gensurf
    << setw(15) << exposure_time
    << "\n"
    << string (100, '=')
    << std::endl;

    TCanvas* c0 = new TCanvas();
    c0->Divide(3,2);
    c0->cd(1);
    detEvtTree->Draw("daqEvents.t", "daqEvents.channel == 0");
    detEvtTree->Draw("daqEvents.t", "daqEvents.channel == 1", "same");

    c0->cd(2);
    gPad->SetLogy();
    detEvtTree->Draw("daqEvents.waveform.peakAmplitude", "daqEvents.channel == 0");
    detEvtTree->Draw("daqEvents.waveform.peakAmplitude", "daqEvents.channel == 1", "same");

    c0->cd(3);
    gPad->SetLogy();
    detEvtTree->Draw("daqEvents.waveform.charge", "daqEvents.channel == 0");
    detEvtTree->Draw("daqEvents.waveform.charge", "daqEvents.channel == 1", "same");

    c0->cd(4);
    gPad->SetLogy();
    detEvtTree->Draw("daqEvents.waveform.peakAmplitude", "daqEvents.channel == 0 && coincidence");
    detEvtTree->Draw("daqEvents.waveform.peakAmplitude", "daqEvents.channel == 1 && coincidence", "same");

    c0->cd(5);
    gPad->SetLogy();
    detEvtTree->Draw("dt", "");

    c0->cd(6);
    gPad->SetLogy();
    detEvtTree->Draw("dt", "");

    TCanvas* c = new TCanvas();
    c->Divide(3,2);
    c->cd(1);
    genTree->Draw("fGenXPos:fGenYPos", "", "surf1z");
    c->cd(2);
    genTree->Draw("fGenXPos:fGenYPos", "isDetected == 1", "surf1z");
    c->cd(3);
    genTree->Draw("fGenTheta:fGenPhi", "", "colz");
    c->cd(4);
    genTree->Draw("fGenTheta");
    c->cd(5);
    genTree->Draw("fGenTheta:log10(fGenEnergy)", "", "colz1");


    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

    return 0;
}
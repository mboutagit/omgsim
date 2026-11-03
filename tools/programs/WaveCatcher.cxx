#include "BWaveCatcher.hh"

#include <CLI/CLI.hpp>
#include <sys/resource.h>

#include <TApplication.h>
#include <TRint.h>
#include <TSystem.h> 

#include "TCanvas.h"
#include "TGraph.h"

#include <csignal>

#include "BLactelDetector.hh"
#include "DAQManager.hh"

#include "TH1.h"
#include "TH2.h"


using namespace lactel;
using namespace std;
using namespace tools;
using namespace DAQ;

int main(int argc, char* argv[]){
    
    std::string input_file, data_file, om, detFile;
    double thr;

    CLI::App cmdApp{"Command-line parser"};

    cmdApp.add_option("-f,--input-file", input_file, "input_file containing DetectedEvent")->required(); // directory containing test files
    cmdApp.add_option("-d,--detector", detFile, "detector file")->required(); // directory containing test files
    cmdApp.add_option("-t,--thr", thr, "threshold")->required(); // directory containing test files

    CLI11_PARSE(cmdApp, argc, argv);
    TApplication app("ROOTApp", &argc, argv);

    TH2F* hist = new TH2F("hist", "test",
                            9, 1, 9., 
                            2, 1, 3.
                        );

    TH2F* hist2 = new TH2F("hist2", "test",
                        100, 0, 10, 
                        100, 0, 3.
                    );
    TH1F* h1 = new TH1F("h1", "modules",
                        100, 0, 10
                    );

    TH1F* oms = new TH1F("oms", "OMs",
                        100, 0, 20
                    );

    TFile* inFile = new TFile(input_file.data(), "OLD");
    BLactelDetector detector;
    BAnalysisManager::DetectedEvent* detectedEvent = nullptr;

    TTree* eventTree = (TTree*)inFile->Get("DetectedEvent");
    TTree* genTree = (TTree*)inFile->Get("Gen");
    // eventTree->Print();
    eventTree->SetBranchAddress("DetectedEvent", &detectedEvent);

   
    if(!fs::exists(detFile)){
        std::cout << "\r\033[10C" << "Could not find detector file" << std::endl;
        exit(-1);
    }
    detector.read(detFile);
    detector.dumpDetectorSummary();
    const int nModules = detector.getListOfModules().size();

    DAQManager* daqManager = DAQManager::getIt();
    std::vector<BWaveCatcher> waveCatchers;

    for(auto&m : detector.getListOfModules()){
        BWaveCatcher wc("wc_M" + std::to_string(m->getID()));
        waveCatchers.push_back(wc);
    }

    std::filesystem::path p(input_file);
    std::string output_name = "daq." + p.filename().string();
    TFile* output = new TFile(output_name.c_str(), "recreate");
    TTree* outTree = new TTree("DetectedEvent", "DAQEvent from DetectedEvent");
    BAnalysisManager::DetectedEvent outEvent;
    outTree->Branch("DetectedEvent", &outEvent);

    // plug OMs first to wcs

      for(auto& wc : waveCatchers){
            for(auto& m : detector.getListOfModules()){
                if(wc.getID() != m->getID())
                    continue;
                int channel = 0;
                for(auto& cell : m->GetObjects()){
                    wc.plugOM(channel, cell.getOM());
                    channel++;
                }
            }
            wc.print();
        }

    // digitize events on all wavecatchers
    for(int i = 0; i < eventTree->GetEntries(); i++){
        eventTree->GetEntry(i);
        for(auto& wc : waveCatchers){
            wc.digitizeEvent(*detectedEvent);
        }

        for(auto& hit : outEvent.hits){
            ElementaryCell* cell = detector.getOMECell(hit.om_id);
            if(! cell)
                continue;
            // std::cout << "\r\033[10C" << "Cell: " << cell->getID() << " found for OM " << hit.om_id << std::endl;
            hist->Fill(cell->getModule(), cell->getLayer());
            hist2->Fill(cell->getModule(), cell->getLayer());
            h1->Fill(cell->getModule());
            oms->Fill(hit.om_id);


        }

        outEvent = *detectedEvent;
        outTree->Fill();
    }
        

    int ncells = detector.getNCells();
    std::cout << "\r\033[10C" << "NCells: " << detector.getNCells() 
                                << " - NMod: " << detector.getNModules() 
                                << std::endl;


    // if(detector.getOMECell(2))
    //     detector.getOMECell(2)->dump();
    // for(int i=0; i <outTree->GetEntries(); i++){
    //     outTree->GetEntry(i);

    //     for(auto& hit : outEvent.hits){
    //         ElementaryCell* cell = detector.getOMECell(hit.om_id);
    //         if(! cell)
    //             continue;
    //         hist->Fill(cell->getModule(), cell->getLayer());
    //         // std::cout << "\r\033[10C" << "mod: " << cell->getModule() 
    //         //                             << " - cell: " << cell->getLayer() 
    //         //                             << std::endl;
    //     }

    //     // if(outEvent.daqEvent.size() >= ncells/2.)
    //     //     std::cout << "\r\033[10C" << "event: " << outEvent.event_id << " has hitted at least half of the detector!"  << std::endl;

    //     // if(outEvent.daqEvent.size() >= ncells/4.)
    //     //     std::cout << "\r\033[10C" << "event: " << outEvent.event_id << " has hitted at least 1/4 of the detector!"  << std::endl;

    //     // if(outEvent.daqEvent.size() >= ncells/8.)
    //     //     std::cout << "\r\033[10C" << "event: " << outEvent.event_id << " has hitted at least 1/8 of the detector!"  << std::endl;

    //     // if(outEvent.daqEvent.size() >= 2)
    //     //     std::cout << "\r\033[10C" << "event: " << outEvent.event_id << " has hitted at least 2 cells of the detector!"  << std::endl;
    // }

    daqManager->dump();
    outTree->Write();
    TTree *newTree = genTree->CloneTree(-1); // copy all entries
    newTree->Write();

    new TCanvas();

    TGraph* detLayout = detector.drawMZ();
    detLayout->Draw("AP");

    TAxis* x = hist->GetXaxis();
    TAxis* y = hist->GetYaxis();
    for (int i = 1; i <= hist->GetNbinsX(); i++)
        x->SetBinLabel(i, ("M" + to_string(i)).c_str());
    
    for (int i = 1; i <= hist->GetNbinsY(); i++)
        y->SetBinLabel(i, ("Layer" + to_string(i)).c_str());
    
    new TCanvas();
    hist->Draw("colz");

    new TCanvas();
    hist2->Draw("");

    new TCanvas();
    h1->Draw("");
  
    new TCanvas();
    oms->Draw("");
    // detLayout->Write();
    // hist->Write();

    
    

    
    std::cout << "\r\033[10C" << "\nWaiting for user action.." << std::endl;
    
    output->Close();
    app.Run();
    return 0;
}
 
#include "BPostAnalysis.hh"
#include "utils.hh"
#include "BPrimaryGeneratorAction.hh"
#include "TF2.h"
#include "BGenerator.hh"
#include "BLactelDetector.hh"

#include "BWaveCatcher.hh"

#include <CLI/CLI.hpp>
#include <sys/resource.h>

using namespace km3net;
using namespace lactel;
using namespace UTILS;
using namespace utils;
using namespace tools;


int main(int argc, char* argv[]){

    std::string input_file, om, detFile, outFile="";
    float thr = -1;

    CLI::App cmdApp{"Command-line parser"};

    cmdApp.add_option("-f,--input-file", input_file, "input_file containing DetectedEvent")->required(); // directory containing test files
    cmdApp.add_option("-d,--detector", detFile, "detector file")->required(); // directory containing test files
    cmdApp.add_option("-t,--thr", thr, "threshold"); // directory containing test files
    cmdApp.add_option("-o,--output", outFile, "ouput file"); // directory containing test files

    CLI11_PARSE(cmdApp, argc, argv);

    TApplication app("PostAna", &argc, argv);

    string rootfilename =   input_file; //app.InputFiles()->At(0)->GetName(); //"../../default.root";
    TFile* file = TFile::Open(rootfilename.c_str(), "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << rootfilename << std::endl;
        return 1;
    }
    else{
    std::cout << "File " << rootfilename << " is open" << std::endl;
    }

    BLactelDetector& detector = BDetectorConstruction::GetIt()->getDetector();
    detector.read(detFile);
    detector.dumpDetectorSummary();

    TTree* genTree = ((TTree*)file->Get("Gen"))->CloneTree();
    TTree* detEvtTree = ((TTree*)file->Get("DetectedEvent"))->CloneTree();
    if(! genTree || !detEvtTree)
        throw runtime_error("Could not find necessary trees.");
    
    if(thr == -1){
        cout << "Threshold not spiecified. using 10 mV." << endl;
        thr = 10;
    }

    if(outFile == "")
        outFile = "BDAQ." + input_file;

    auto* event= static_cast<BAnalysisManager::DetectedEvent*>(nullptr);
    detEvtTree->SetBranchAddress("DetectedEvent", &event);

    std::vector<BAnalysisManager::DAQEvent> daqEventVect;
    TBranch* branch = detEvtTree->Branch("AdaqEvent", &daqEventVect);

    // >>>>  prepare waveCatchers.
    int m = 0;
    BWaveCatcher* waveCatchers[detector.getListOfModules().size()];
    for(auto& module : detector.getListOfModules()){
        waveCatchers[m] = new BWaveCatcher("wc_" + to_string(m), m);
        waveCatchers[m]->setTriggerThreshold(thr);
        waveCatchers[m]->getConfiguration().setInputDynamicRange(6);
        int c = 0;
        for(auto& cell : module->getElementaryCells()){
            waveCatchers[m]->plugOM(c, cell.getOM());
            c++;
        }
        // waveCatchers[m]->print();
        m++;
    }

    // <<<< prepare waveCatchers.

    TFile* output = new TFile(outFile.c_str(), "RECREATE");

    output->cd();

    for(long long i =0; i < detEvtTree->GetEntries(); i++){
        detEvtTree->GetEntry(i);
        daqEventVect.clear();
        for(auto& wc : waveCatchers){
            wc->digitizeEvent(*event);
            
            for(int i = 0; i < wc->getConfiguration().NumChannels; i++){
                BAnalysisManager::DAQEvent daqEvt;
                daqEvt.channel = i;
                if(i < wc->getWaveforms().size()){
                    daqEvt.amp = wc->getWaveforms()[i].minimum;
                    daqEvt.t = wc->getWaveforms()[i].leadingEdge;
                    daqEvt.c = wc->getWaveforms()[i].charge;
                    daqEventVect.push_back(daqEvt);  
                }           
            }
        }
        branch->Fill();
    }

    detEvtTree->Write();
    genTree->Write();

    // detEvtTree->Write();
    output->Close();

    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

}
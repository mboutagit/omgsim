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

    TTree* genTree = ((TTree*)file->Get("Gen"));
    TTree* detEvtTree = ((TTree*)file->Get("DetectedEvent"));

    if(!genTree || !detEvtTree)
        throw runtime_error("Sorry, Could not find necessary trees.");
    
    if(thr == -1){
        cout << "Threshold not spiecified. using 10 mV." << endl;
        thr = 10;
    }

    if(outFile == ""){
        std::string filename = std::filesystem::path(input_file).filename();
        outFile = "BDAQ." + filename;
    }
        

    auto* event = static_cast<BAnalysisManager::DetectedEvent*>(nullptr);
    detEvtTree->SetBranchAddress("DetectedEvent", &event);

    // >>>>  prepare waveCatchers.
    int m = 0;
    BWaveCatcher* waveCatchers[detector.getListOfModules().size()];
    for(auto& module : detector.getListOfModules()){
        waveCatchers[m] = new BWaveCatcher("wc_" + to_string(m));
        waveCatchers[m]->setTriggerThreshold(thr);
        waveCatchers[m]->getConfiguration().setInputDynamicRange(6);
        waveCatchers[m]->setTriggerEdge(BWaveCatcherConf::TriggerEdge::Falling);
        waveCatchers[m]->activate();
        int c = 0;
        for(auto& cell : module->getElementaryCells()){
            waveCatchers[m]->plugOM(c, cell.getOM());
            c++;
        }
        waveCatchers[m]->print();
        m++;
    }

    // <<<< prepare waveCatchers.

    TFile* output = new TFile(outFile.c_str(), "RECREATE");
    output->cd();
    
    TTree* physicEventTree = new TTree("physicsEvent", "DAQ and detected Event");
    TTree* genTreeCopy = ((TTree*)file->Get("Gen"))->CloneTree();


    auto* physicsEvent = static_cast<BAnalysisManager::DetectedEvent*>(nullptr);
    TBranch* branch = physicEventTree->Branch("DAQ", &physicsEvent);

    for(long long i =0; i < detEvtTree->GetEntries(); i++){
        detEvtTree->GetEntry(i);
        // daqEventVect.clear();
        physicsEvent = new BAnalysisManager::DetectedEvent(*event);
        for(auto& wc : waveCatchers){
            wc->digitizeEvent(*physicsEvent);
        }
        physicEventTree->Fill();

        delete physicsEvent;
        physicsEvent = nullptr;
    }
    physicEventTree->Write();
    genTreeCopy->Write();

    file->Close();
    output->Close();

    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

}
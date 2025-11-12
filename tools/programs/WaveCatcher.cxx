#include "BWaveCatcher.hh"

#include <CLI/CLI.hpp>
#include <sys/resource.h>

#include <TApplication.h>
#include <TRint.h>
#include <TSystem.h> 

#include <csignal>


using namespace lactel;
using namespace std;
using namespace tools;


int main(int argc, char* argv[]){
    
    std::string input_file, data_file, om;
    double thr;

    CLI::App cmdApp{"Command-line parser"};

    cmdApp.add_option("-f,--input-file", input_file, "input_file containing DetectedEvent")->required(); // directory containing test files
    cmdApp.add_option("-t,--thr", thr, "threshold")->required(); // directory containing test files

    CLI11_PARSE(cmdApp, argc, argv);
    TApplication app("ROOTApp", &argc, argv);
    gSystem->Load("libLACTELSim.so");
    gSystem->Load("liblibOMG.so");

    BWaveCatcher *wc = new BWaveCatcher();
    wc->startAcquisition();

    

    

    


    app.Run();
    return 0;
}

#include "BWaveCatcher.hh"

#include <CLI/CLI.hpp>
#include <sys/resource.h>

#include <TApplication.h>
#include <TRint.h>
#include <TSystem.h> 

using namespace lactel;
using namespace std;


int main(int argc, char* argv[]){
    
    std::string input_file, data_file, om;
    double thr;

    CLI::App cmdApp{"Command-line parser"};

    cmdApp.add_option("-f,--input-file", input_file, "input_file containing DetectedEvent")->required(); // directory containing test files
    cmdApp.add_option("-t,--thr", thr, "threshold")->required(); // directory containing test files

    CLI11_PARSE(cmdApp, argc, argv);

    TRint app("ROOTApp", &argc, argv);
    gSystem->Load("libLACTELSim.so");
    gSystem->Load("liblibOMG.so");



    std::cout << "\033[1;35mWaiting for user action...\033[0m" << std::endl;
    std::cout << "\033[5;35m\tPress ctrl-c to exit\033[0m" << std::endl;
    app.Run();
    return 0;
}

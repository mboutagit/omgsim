#include "BPostAnalysis.hh"

using namespace km3net;
using namespace lactel;


int main(int argc, char* argv[]){

    TApplication app("ROOTApp", &argc, argv);
    gSystem->Load("liblibOMG.so");


    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

    return 0;
}
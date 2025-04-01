#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TKey.h>
#include <TLeaf.h>
#include <TCollection.h> 
#include <TCanvas.h> 
#include <TH1F.h> 
#include <TH2F.h> 
#include <TH3F.h> 
#include <TApplication.h>
#include <TPolyLine3D.h>
#include <TArrow.h>

#include <TSystem.h>  // For gSystem->ProcessEvents()
#include <unistd.h>   // For usleep()


#include "BPostAnalysis.hh"

using namespace km3net;


int main(int argc, char* argv[]){

  TApplication app("ROOTApp", &argc, argv);
  gSystem->Load("liblibOMG.so");


    if(argc < 1){
    cout << "Syntax error: expected root file name" << endl;
    return 1;
    }

    string rootfilename =   app.InputFiles()->At(0)->GetName(); //"../../default.root";
      TFile* file = TFile::Open(rootfilename.c_str(), "READ");

      if (!file || file->IsZombie()) {
          std::cerr << "Error opening file: " << rootfilename << std::endl;
          return 1;
      }
      else{
        std::cout << "File " << rootfilename << " is open" << std::endl;
      }
    
    file->Print();

    TTree* genTree = (TTree*)file->Get("Gen");
    TTree* hitTree = (TTree*)file->Get("Hit");

    BAnalysis::GenerationParameters* gps;
    BAnalysis::Event* evt;
    PhotonProduction pp;
    Hit hit;

    genTree->SetBranchAddress("gen", &gps);
    genTree->SetBranchAddress("Event", &evt);
    hitTree->SetBranchAddress("Hit", &hit);

    TCanvas *c = new TCanvas("c", "Arrow Example", 800, 600);

    TH3F *dir = new TH3F("dir","Dir; X; Y; Z",  300, -8000, 8000,
                                                300, -8000, 8000,
                                                300, -3000, 3000);

    std::vector<TH2F*> EvsTheta;
    std::vector<TH1F*> E_theta;
    std::vector<float> angles = {90, 100, 110, 120, 130, 140, 150, 160, 170, 180};

    TH2F* his_all_events = new TH2F("his_all_events","Pos; X (mm); Y (mm)", 
                                    500, -8000, 8000,
                                    500, -8000, 8000
                                  );
    TH2F* his_det_events = new TH2F("his_det_events","Pos; X (mm); Y (mm)", 
                                    500, -8000, 8000,
                                    500, -8000, 8000
                                  );


    cout << "\r\033[20C" << "genTree Entries: " << genTree->GetEntries() << endl;
    for(Int_t i=0; i< genTree->GetEntries(); i++){
        genTree->GetEntry(i);

        for (int k = 0; k < 1000; k++)
        {
          dir->Fill(gps->fGenXPos + k * gps->fGenXDir, 
                    gps->fGenYPos + k * gps->fGenYDir, 
                    gps->fGenZPos + k * gps->fGenZDir
                  );
        }

        his_all_events->Fill(gps->fGenXPos, gps->fGenYPos);
        if(evt->isDetected)
          his_det_events->Fill(gps->fGenXPos, gps->fGenYPos);
    }

    his_all_events->SetMarkerColor(3);
    his_det_events->SetMarkerColor(2);

    his_all_events->SetMarkerStyle(20);
    his_det_events->SetMarkerStyle(20);

    his_all_events->Draw("scat");
    his_det_events->Draw("scat same");

    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

    return 0;



}
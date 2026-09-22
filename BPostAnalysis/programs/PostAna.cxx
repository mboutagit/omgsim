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

 

  std::string input_file, om, detFile;
  float thr;

  CLI::App cmdApp{"Command-line parser"};

  cmdApp.add_option("-f,--input-file", input_file, "input_file containing DetectedEvent")->required(); // directory containing test files
  cmdApp.add_option("-d,--detector", detFile, "detector file")->required(); // directory containing test files
  cmdApp.add_option("-t,--thr", thr, "threshold")->required(); // directory containing test files

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

  // BPostAnalysis::detector.read(detFile);
  BLactelDetector& detector = BDetectorConstruction::GetIt()->getDetector();
          std::cout << "\r\033[10C" << "DC 1 " << BDetectorConstruction::GetIt() << std::endl;
  detector.read(detFile);
  detector.dumpDetectorSummary();
  // gSystem->Load("liblibOMG.so");
    // file->Print();

    TTree* genTree = (TTree*)file->Get("Gen");
    // TTree* hitTree = (TTree*)file->Get("Hit");
    // TTree* runTree = (TTree*)file->Get("Run");
    TTree* detEvtTree = (TTree*)file->Get("DetectedEvent");

    // BAnalysisManager::GenerationParameters* gps = nullptr;
    // BAnalysisManager::DetectedEvent* evt = nullptr;
    // PhotonProduction* pp;
    // Hit* hit;
    // BAnalysisManager::Run* run;

    // genTree->SetBranchAddress("gen", &gps);
    // hitTree->SetBranchAddress("Hit", &hit);
    // runTree->SetBranchAddress("Run", &run);
    // detEvtTree->SetBranchAddress("DetectedEvent", &evt);
    // detEvtTree->Print();


    // TF2* generator = new TF2("modFlux2D", 
    //                           [](double* x, double* pars){
    //                               return 
    //                               lactel::BPrimaryGeneratorAction::ModifiedFormula2D(x, pars);
    //                           },
    //                           1e-3, 1e3, 0, 70
    //                       );

     TF2* generator = new TF2("modFlux2D", 
                              [](double* x, double* pars){
                                  return 
                                  BGenerator::RateIntegrand(x, pars);
                              },
                              1e-3, 1e3, 0, TMath::Pi()/2.
                          );
            
    

    float radius = 5; // in meters    
    float gen_surface_m2  = TMath::Pi() * radius*radius; // pi*R² disk
    float gen_surface_cm2 = gen_surface_m2 * 1e4;
 
    float expected_muons_per_m2_per_s = 130; //generator->Integral(1e-1, 1e3, 0, TMath::Pi()/2); // by m²
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

    histsByName hists_data;
    histsByName hists_mc;
    std::vector<TCanvas*>  canvas_data =  getRealDataPlots(hists_data,
                                                            "/home/bouta-mohammed/work/cppm/lactel/realData/data/run_003/data.h5",
                                                            "/home/bouta-mohammed/work/cppm/lactel/realData/data/run_004/data.h5"); 

    std::vector<TCanvas*>  canvas_mc =  getNPEPlot(detEvtTree, hists_mc, exposure_time, 15, -0.005);
    TCanvas* verPosition =  getVertexPositions(detEvtTree);
    verPosition->Draw();

    // std::vector<TCanvas*> canvas = getNPEPlot(detEvtTree, evt, exposure_time, 1e7);
    // for(auto& c : canvas)
    //   c->Draw();


    new TCanvas();


 ///////////////// Normal
    TCanvas* cs = new TCanvas("cs", "Normal Trigger");
    cs->SetLogy();
    for(auto& entry : hists_data){
      int pos = entry.first.find("amp_ch0_run3");
      if(pos != std::string::npos){
        entry.second->SetLineColor(1);
        entry.second->Draw("E1 hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
        
    }

    for(auto& entry : hists_mc){
      int pos = entry.first.find("mc_amp_ch0_run3"); // find amplitude hists
      if(pos != std::string::npos){
        entry.second->SetLineColor(2);
        entry.second->Draw("hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
    }


     ///////////////// Coin

    TCanvas* cnCoin = new TCanvas("cnCoin", "Coinsidence");
    cnCoin->SetLogy();
    for(auto& entry : hists_data){
      int pos = entry.first.find("amp_ch0_run4");
      if(pos != std::string::npos){
        entry.second->SetLineColor(1);
        entry.second->Draw("E1 hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
        
    }

    for(auto& entry : hists_mc){
      int pos = entry.first.find("mc_amp_ch0_run4"); // find amplitude hists
      if(pos != std::string::npos){
        entry.second->SetLineColor(2);
        entry.second->Draw("hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
    }

     ///////////////// Normal
    TCanvas* cnNtrigCh1 = new TCanvas("cnNtrigCh1", "Normal Trigger ch1");
    cnNtrigCh1->SetLogy();
    for(auto& entry : hists_data){
      int pos = entry.first.find("amp_ch1_run3");
      if(pos != std::string::npos){
        entry.second->SetLineColor(1);
        entry.second->Draw("E1 hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
        
    }

    for(auto& entry : hists_mc){
      int pos = entry.first.find("mc_amp_ch1_run3"); // find amplitude hists
      if(pos != std::string::npos){
        entry.second->SetLineColor(2);
        entry.second->Draw("hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
    }

    ///////////////// Coin

    TCanvas* cnCoinCh1 = new TCanvas("cnCoinCh1", "Coinsidence ch1");
    cnCoinCh1->SetLogy();
    for(auto& entry : hists_data){
      int pos = entry.first.find("amp_ch1_run4");
      if(pos != std::string::npos){
        entry.second->SetLineColor(1);
        entry.second->Draw("E1 hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
        
    }

    for(auto& entry : hists_mc){
      int pos = entry.first.find("mc_amp_ch1_run4"); // find amplitude hists
      if(pos != std::string::npos){
        entry.second->SetLineColor(2);
        entry.second->Draw("hist same");
        std::cout << "\r\033[10C" << "Integral for hist: " << entry.second->GetName() << " is: " << entry.second->Integral() << std::endl;
      }
    }













    TCanvas *c = new TCanvas("c", "Ratio Canvas");
    c->SetLogy();
    TGraph* ratio_om0_r3 = lactel::ratio(getHistByName(hists_data, "amp_ch0_run3"), getHistByName(hists_mc, "mc_amp_ch0_run3"));
    TGraph* ratio_om0_r4 = lactel::ratio(getHistByName(hists_data, "amp_ch0_run4"), getHistByName(hists_mc, "mc_amp_ch0_run4"));
    TGraph* ratio_om1_r3 = lactel::ratio(getHistByName(hists_data, "amp_ch1_run3"), getHistByName(hists_mc, "mc_amp_ch1_run3"));
    TGraph* ratio_om1_r4 = lactel::ratio(getHistByName(hists_data, "amp_ch1_run4"), getHistByName(hists_mc, "mc_amp_ch1_run4"));
    ratio_om0_r3->SetTitle("MC(OM0) / Data (OM0); Amplitude [V]; Ratio");
    ratio_om0_r3->SetLineColor(1);
    ratio_om0_r3->SetMarkerColor(1);
    ratio_om0_r3->SetMarkerStyle(20);

    ratio_om0_r4->SetLineColor(3);
    ratio_om0_r4->SetMarkerColor(3);
    ratio_om0_r4->SetMarkerStyle(20);

    ratio_om1_r3->SetLineColor(2);
    ratio_om1_r3->SetMarkerColor(2);
    ratio_om1_r3->SetMarkerStyle(20);

    ratio_om1_r4->SetLineColor(4);
    ratio_om1_r4->SetMarkerColor(4);
    ratio_om1_r4->SetMarkerStyle(20);

    ratio_om0_r3->Draw("AP");
    ratio_om0_r4->Draw("P same");

        TCanvas *rch1 = new TCanvas("rch1", "Ratio Canvas ch1");

    ratio_om1_r3->Draw("AP");
    ratio_om1_r4->Draw("P same");

    BSquare* del = new BSquare(0, ratio_om0_r3->GetXaxis()->GetXmax(), 0.8, 1.2, 2, 6);
    del->Draw();

    TCanvas *c1 = new TCanvas("c1", "Gen");
    genTree->Draw("cos(fGenTheta*3.14/180):log10(fGenEnergy/1e3)", "", "colz");

    TCanvas *c2 = new TCanvas("c2", "Gen Detected");
    genTree->Draw("cos(fGenTheta*3.14/180):log10(fGenEnergy/1e3)", "isDetected==1", "colz");


    TIter next(gROOT->GetListOfCanvases());
    TCanvas* allCanvas;
    while ((allCanvas = (TCanvas*)next())) {
        cout << "Canvas: " << c->GetName() << endl;
    }


    histsByName histDataRun003;
    getRealDataByInFile(  histDataRun003,
                          "/home/bouta-mohammed/work/cppm/lactel/realData/data/run_003/data.h5",
                          "channel_data/peak",
                          0, 2.4, BPostAnalysis::mVperBin/1e3,
                          1);


    histsByName histDataRun004;
    getRealDataByInFile(  histDataRun004,
                          "/home/bouta-mohammed/work/cppm/lactel/realData/data/run_004/data.h5",
                          "channel_data/peak",
                          0, 2.4, BPostAnalysis::mVperBin/1e3,
                          0.5);

    histsByName histDataRun000020;
    getRealDataByInFile(  histDataRun000020,
                          "/home/bouta-mohammed/work/cppm/lactel/realData/data/run_000020/data.h5",
                          "channel_data/peak",
                          0, 2.4, BPostAnalysis::mVperBin/1e3,
                          0.5);


    TCanvas* checkCanvas_ch = new TCanvas("check", "check");
    checkCanvas_ch->SetLogy();

    // channel 0
    TH1* ampCh0_run0003 = getHistByName(histDataRun003, "amp_ch0");
    ampCh0_run0003->SetLineColor(1);
    ampCh0_run0003->Draw("hist");
    TH1* ampCh0_run000020 = getHistByName(histDataRun004, "amp_ch0");
    ampCh0_run000020->SetLineColor(1);
    ampCh0_run000020->SetLineStyle(2);
    ampCh0_run000020->Draw("hist same");

    // channel 1
    TH1* ampCh1_run0003 = getHistByName(histDataRun003, "amp_ch1");
    ampCh1_run0003->SetLineColor(2);
    ampCh1_run0003->Draw("hist same");
    TH1* ampCh1_run000020 = getHistByName(histDataRun004, "amp_ch1");
    ampCh1_run000020->SetLineColor(2);
    ampCh1_run000020->SetLineStyle(2);
    ampCh1_run000020->Draw("hist same");

    // TCanvas* makeACustumPlot =  makeRatioPlot(ampCh0_run0003, ampCh0_run0003);
    // TCanvas* makeACustumPlot1 =  makeRatioPlot(ampCh1_run0003, ampCh1_run0003);

    DrawWithRatio(getHistByName(hists_mc, "mc_amp_ch0_run4"), ampCh0_run000020, "r.coin.ch0");
    DrawWithRatio(getHistByName(hists_mc, "mc_amp_ch1_run4"), ampCh1_run000020, "r.coin.ch1");
    DrawWithRatio(getHistByName(hists_mc, "mc_amp_ch0_run3"), 
                                  getHistByName(histDataRun000020, "amp_ch0"), 
                                  "rtr.Amp.ch1"
                                );

    DrawWithRatio( {
                      {getHistByName(hists_mc, "mc_amp_ch0_run4"), ampCh0_run000020},
                      {getHistByName(hists_mc, "mc_amp_ch1_run4"), ampCh1_run000020},
                    },
                  "r.coinComp.ch0ch1"
                );

              





    TCanvas* ratioCanvas = new TCanvas("ratio", "Ratio");
    ratioCanvas->SetLogy();
    
    ampCh0_run0003->Sumw2(1);
    ampCh0_run000020->Sumw2(1);
    TH1* ratio = (TH1*)ampCh0_run0003->Clone("ratio");
    ratio->Divide(ampCh0_run000020);
    ratio->SetLineColor(4);
    ratio->Draw("hist same");

    //channel 1

    ampCh1_run0003->Sumw2(1);
    ampCh1_run000020->Sumw2(1);
    TH1* ratioCh1 = (TH1*)ampCh1_run0003->Clone("ratio");
    ratioCh1->Divide(ampCh1_run000020);
    ratioCh1->SetLineColor(4);
    ratioCh1->SetLineStyle(2);
    ratioCh1->Draw("hist same");

      
    // Run 3: rate: 1185 evt/s, dead time: 79%
    // Run 4: rate: 57.5 evt/s, dead time: 3.8%


    // getDAQParameter(detEvtTree, evt, "Amplitude")->Draw();







    // // TCanvas *c = new TCanvas("c", "Arrow Example", 800, 600);

    // TH3F *dir = new TH3F("dir","Dir; X; Y; Z",  300, -8000, 8000,
    //                                             300, -8000, 8000,
    //                                             300, -3000, 3000
    //                       );
                                                
    // // TH3F *dirDet = new TH3F("dirDet","Dir; X; Y; Z",  300, -8000, 8000,
    // //                                             300, -8000, 8000,
    // //                                             300, -3000, 3000);

    // std::vector<TH2F*> EvsTheta;
    // std::vector<TH1F*> E_theta;
    // std::vector<float> angles = {10, 20, 30, 40, 50, 60};
    // std::vector<std::string> detCong = {"M2", "M4", "M6", "M8", "M10"};

    // TH2F* his_all_events = new TH2F("his_all_events","Pos; X (mm); Y (mm)", 
    //                                 500, -5000, 5000,
    //                                 500, -5000, 5000
    //                               );
    // TH2F* his_det_events = new TH2F("his_det_events","Pos; X (mm); Y (mm)", 
    //                                 500, -5000, 5000,
    //                                 500, -5000, 5000
    //                               );

    // TH1F* his_hits = new TH1F("his_hits","Hit charge distribution; hit charge (p.e); Count", 
    //                                 500, 1, 5
    //                               );


    // cout << "\r\033[20C" << "genTree Entries: " << genTree->GetEntries() << endl;


    // std::vector<TPolyLine3D*> DetectedTracks;
    // std::vector<TPolyLine3D*> UnDetectedTracks;
   
   
    // int nDetected = 0;
    // int dl = 1500;
    // for(long long i = 0; i < genTree->GetEntries(); i++){
    //     genTree->GetEntry(i);

    //     float x, y, z, xd, yd, zd;
    //     x = gps->fGenXPos;
    //     y = gps->fGenYPos;
    //     z = gps->fGenZPos;
    //     xd = gps->fGenXDir;
    //     yd = gps->fGenYDir;
    //     zd = gps->fGenZDir;
        




    //     std::vector<BAnalysisManager::Hit> hits = evt->hits;
    //     for(auto& hit : hits){
    //       his_hits->Fill(hit.charge);
    //     }
          
        
    //     // cout << "\r\033[10C" << "test here: is detected = " << evt->isDetected << endl;
    //     // cout << "\r\033[10C" << "posX: " << gps->fGenXPos << " - posY: " << gps->fGenYPos << " - is det: " << evt->isDetected << endl;
    //     //if(evt->isDetected)
    //       // for (long  k = 0; k < 100; k++)
    //       // {
    //       //   dirDet->Fill(gps->fGenXPos + 10*k * gps->fGenXDir, 
    //       //             gps->fGenYPos + 10*k * gps->fGenYDir, 
    //       //             gps->fGenZPos + 10*k * gps->fGenZDir
    //       //           );
    //       // }
    //     // else{
    //       dir->Fill(x, y, z);
    //       //}
           
    //     //if(evt->isDetected){          
    //       TPolyLine3D *aTrack = new TPolyLine3D(2);  // 2 points = 1 line segment
    //       aTrack->SetPoint(0, x, y, z);
    //       aTrack->SetPoint(1, x + dl*xd, y + dl*yd, z + dl*zd);
    //       DetectedTracks.push_back(aTrack);
    //     // }
    //     // else{
    //       // TPolyLine3D *aTrack = new TPolyLine3D(2);  // 2 points = 1 line segment
    //       // aTrack->SetPoint(0, x, y, z);
    //       // aTrack->SetPoint(1, x + dl*xd, y + dl*yd, z + dl*zd);
    //       // NonDetectedTracks.push_back(aTrack);
    //   //  }
        
        
    //     his_all_events->Fill(x, y);
    //     // if(evt->isDetected)
    //     //   his_det_events->Fill(x, y);
    // }


    // TCanvas c;

    // his_hits->SetLineColor(4);
    // his_hits->Draw();

    // TCanvas c1;

    // TView *v = TView::CreateView(1);
    // v->SetRange(-5000, -5000, -2000,    // min x, y, z
    //             +5000, +5000, +500);   // max x, y, z

    // // dir->Draw("p");

    // // for(long i = 0; i < detEvtTree->GetEntries(); i++){
    // //     TPolyLine3D *aTrack = new TPolyLine3D(2);
    // //     aTrack->SetPoint(0, x, y, z);
    // //     aTrack->SetPoint(1, x + dl*xd, y + dl*yd, z + dl*zd);
    // //     DetectedTracks.push_back(aTrack);
    // // }

    // const Double_t x[8] = {-1000, 1000., 1000., -1000, -1000, 1000., 1000., -1000};
    // const Double_t y[8] = {-1000, -1000, 1000., 1000., -1000, -1000, 1000., 1000.};
    // const Double_t z[8] = {-2000, -2000, -2000, -2000, 0., 0., 0., 0.};
  
    // int edges[12][2] = {
    //   {0,1}, {1,2}, {2,3}, {3,0}, // bottom
    //   {4,5}, {5,6}, {6,7}, {7,4}, // top
    //   {0,4}, {1,5}, {2,6}, {3,7}  // sides
    // };
  
    // // for(auto& track : NonDetectedTracks){
    // //   track->SetLineColorAlpha(7, 0.4);
    // //   track->SetLineWidth(1);
    // //   track->SetLineColor(1);
    // //   track->Draw();
    // // }

    // for(auto& track : DetectedTracks){
    //   track->SetLineColorAlpha(2, 0.4);
    //   track->SetLineWidth(1);
    //   track->SetLineColor(5);
    //   track->Draw("same");
    // }

   
    // for (int i = 0; i < 12; ++i) {
    //   TPolyLine3D* line = new TPolyLine3D(2);
    //   line->SetPoint(0, x[edges[i][0]], y[edges[i][0]], z[edges[i][0]]);
    //   line->SetPoint(1, x[edges[i][1]], y[edges[i][1]], z[edges[i][1]]);
    //   line->SetLineColorAlpha(4, 0.4);
    //   line->SetLineWidth(3);
    //   line->Draw("same");
    // }

    // v->ShowAxis();
    // c1.Update();
    

    // TCanvas c2;

    // his_all_events->SetMarkerColor(7);
    // his_det_events->SetMarkerColor(2);

    // his_all_events->SetMarkerStyle(20);
    // his_det_events->SetMarkerStyle(20);

    // his_all_events->SetMarkerSize(0.3);
    // his_det_events->SetMarkerSize(0.3);

    // his_all_events->Draw("scat");
    // his_det_events->Draw("scat same");


    // std::vector<uint> nGenEvent;
    // std::vector<uint> nDetEvent;
    // std::vector<float> expectedRate;
    // std::vector<float> generationSurface;
    // std::vector<float> acceptanceAngle;
    // std::vector<std::string> detname;

    // std::vector< pair<std::string, pair<float, TGraph*>>> detAccaGraph;

    // for(int i = 0; i < detCong.size(); i++){
    //   for (int j = 0; j < angles.size(); j++)
    //   {
    //       TGraph* graph;
    //       detAccaGraph.emplace_back(detCong[i], make_pair(angles[j], graph));
    //   }
      
    // }
    
    // for(long long i = 0; i < runTree->GetEntries(); i++){
    //     genTree->GetEntry(i);

    //     if(run.acceptanceAngle == 60){
    //       nGenEvent        .push_back(run.nGenEvent);
    //       nDetEvent        .push_back(run.nDetEvent);
    //       expectedRate     .push_back(run.expectedRate);
    //       generationSurface.push_back(run.generationSurface);
    //       acceptanceAngle  .push_back(run.acceptanceAngle);
    //       detname          .push_back(run.detName);
    //     }
    // }

    // TCanvas c3;

    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

    return 0;
}
#include "BPostAnalysis.hh"

using namespace km3net;
using namespace lactel;


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
    
    // file->Print();

    // TTree* genTree = (TTree*)file->Get("Gen");
    TTree* hitTree = (TTree*)file->Get("Hit");
    TTree* runTree = (TTree*)file->Get("Run");
    TTree* detEvtTree = (TTree*)file->Get("DetectedEvent");

    BAnalysisManager::GenerationParameters* gps = nullptr;
    BAnalysisManager::DetectedEvent* evt = nullptr;
    PhotonProduction pp;
    Hit hit;
    BAnalysisManager::Run* run;

    // genTree->SetBranchAddress("gen", &gps);
    hitTree->SetBranchAddress("Hit", &hit);
    runTree->SetBranchAddress("Run", &run);
    detEvtTree->SetBranchAddress("DetectedEvent", &evt);

    float radius = 6.19; // in meters
    float gen_surface_m2  = TMath::Pi() * radius*radius; // pi*R²
    float gen_surface_cm2 = gen_surface_m2 * 1e4;
 
    float expected_muons_per_m2_per_s = 110; // by m²
    float expected_muons_per_cm2_per_s = expected_muons_per_m2_per_s/1e4; // to cm²
    float expected_muons_per_gensurf = expected_muons_per_cm2_per_s * gen_surface_cm2;
    float number_of_generated_muons_per_gensurf = 1e5; // 100 000 events
    float exposure_time = number_of_generated_muons_per_gensurf / expected_muons_per_gensurf;

    std::vector<std::pair<std::string, TH1F*>> hists_data;
    std::vector<std::pair<std::string, TH1F*>> hists_mc;
    std::vector<TCanvas*>  canvas_data =  getRealDataPlots(hists_data);
    std::vector<TCanvas*>  canvas_mc =  getNPEPlot(detEvtTree, evt, hists_mc, exposure_time, 1e7);


    // std::vector<TCanvas*> canvas = getNPEPlot(detEvtTree, evt, exposure_time, 1e7);
    // for(auto& c : canvas)
    //   c->Draw();

    TCanvas* cs = new TCanvas("cs");
    cs->SetLogy();
    for(auto& entry : hists_data){
      entry.second->SetLineStyle(7);
      entry.second->Draw("hist same");
    }

    for(auto& entry : hists_mc){
      int pos = entry.first.find("amp_ch"); // find amplitude hists
      if(pos != std::string::npos)
        entry.second->Draw("hist same");
    }
      


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
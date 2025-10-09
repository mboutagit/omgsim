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
#include <TLine.h>
#include <TView.h>
#include <TBox.h>
#include <TMath.h>

#include <string>


// int main(int arg_v, char** agr_v){

double dtr(double deg){
    return  deg/180. * TMath::Pi();
}

double rtd(double rad){
    return  rad/TMath::Pi() * 180.;
}

using namespace std;
void ana(){

TFile* M2File = new TFile("/home/bouta-mohammed/work/cppm/lactel/trunk/build/results/last/all_M2.root");
TFile* M4File = new TFile("/home/bouta-mohammed/work/cppm/lactel/trunk/build/results/last/all_M4.root");
TFile* M6File = new TFile("/home/bouta-mohammed/work/cppm/lactel/trunk/build/results/last/all_M6.root");


TTree* M2RunTree = (TTree*)M2File->Get("Run");
TTree* M4RunTree = (TTree*)M4File->Get("Run");
TTree* M6RunTree = (TTree*)M6File->Get("Run");

TTree* M2DetectedEventTree = (TTree*)M2File->Get("DetectedEvent");
TTree* M4DetectedEventTree = (TTree*)M4File->Get("DetectedEvent");
TTree* M6DetectedEventTree = (TTree*)M6File->Get("DetectedEvent");

// M2RunTree->Draw("detName");

// TH1F* omzM2 = new TH1F("omzM2", "OM Z Pos.; OM z pos ; Counts", 100, -6, 0);
// TH1F* omzM4 = new TH1F("omzM4", "OM Z Pos.; OM z pos ; Counts", 100, -6, 0);
// TH1F* omzM6 = new TH1F("omzM6", "OM Z Pos.; OM z pos ; Counts", 100, -6, 0);

// M2DetectedEventTree->Draw("OMPosition.data[2]/1000 - 1 >> omzM2", "", "");
// M4DetectedEventTree->Draw("OMPosition.data[2]/1000 - 2 >> omzM4", "", "same");
// M6DetectedEventTree->Draw("OMPosition.data[2]/1000 - 3 >> omzM6", "", "same");


double M2length=2.;
double M4length=4.;
double M6length=6.;

TH2F* hisndngz[31];
int i=0;
for(int ze = 0; ze <= 20; ze+=2){
    ostringstream angle;
    angle << ze;
    std::string hisName = "M2_" + angle.str() + "_hist";
    std::string drawCommand="nDetEvent:nGenEvent >> M2_" + angle.str() + "_hist";
    std::string drawCondition="";
    std::string drawOption="";
    
    std::cout << "\r\033[10C" << "drawCommand: " << drawCommand << std::endl;

    hisndngz[i] = new TH2F(hisName.c_str(), ("his nDet vs nGen" + angle.str() + "; nGen; nDet").c_str(), 100, 24000, 26000, 100, 0, 10000);
    M2RunTree->Draw(drawCommand.c_str(), drawCondition.c_str(), drawOption.c_str());
    // hisndngz[i]->SetStats(0);
    hisndngz[i]->Draw("same");
    i++;
}

// TCanvas* cm2_pmxy = new TCanvas("cm2_pmxy", "Canvas PM M2 OM Z positions");
// cm2_pmxy->Divide(3,3);
// TH2F* hisM2PMxy[7];
// for(int i=1; i<8; i++){
//     std::ostringstream omzp;
//     double pos = -M2length/2. + i * M2length/8.; // minimum + i*step
//     omzp << -M2length/2. + pos;
//     // std::cout << "\r\033[10C" << "M2 position " << i << ": " << omzp.str() << std::endl;
//     std::string hisName=("hisM2PMxy_" + std::to_string(i)); 
//     // std::cout << "\r\033[10C" << "histName: " << hisName << std::endl;
//     std::string drawCommand="hits.pm_x:hits.pm_y >> hisM2PMxy_" + std::to_string(i);
//     // std::cout << "\r\033[10C" << "drawCommand: " << drawCommand << std::endl;
//     std::string drawCondition="OMPosition.data[2]/1000 == " + std::to_string(pos);
//     std::cout << "\r\033[10C" << "drawCondition: " << drawCondition << std::endl;

//     cm2_pmxy->cd(i);
//     hisM2PMxy[i-1] = new TH2F(hisName.c_str(), ("his pm hits xy for omz " + omzp.str() + "m w.r.t water tank bottom; x (mm); y (mm)").c_str(), 100, -150, 150, 100, -150, 150);
//     M2DetectedEventTree->Draw(drawCommand.c_str(), drawCondition.c_str()); 
//     hisM2PMxy[i-1]->SetStats(0);
//     hisM2PMxy[i-1]->Draw("colz");
// } 

// TCanvas* cm2xz = new TCanvas("cm2xz", "Canvas PM XZ M2 OM Z positions");
// cm2xz->Divide(3,3);
// TH2F* hisM2PMxz[7];

// for(int i=1; i<8; i++){
//     std::ostringstream omzp;
//     double pos = -M2length/2. + i * M2length/8.; // minimum + i*step
//     omzp << -M2length/2. + pos;
//     std::string hisNamexz=("hisM2PMxz_" + std::to_string(i)); 
//     std::string drawCommandxz="hits.pm_z:hits.pm_x >> hisM2PMxz_" + std::to_string(i);
//     std::string drawCondition="hits.charge * (OMPosition.data[2]/1000 == " + std::to_string(pos) + ")";
//     std::cout << "\r\033[10C" << "drawCondition: " << drawCondition << std::endl;

//     cm2xz->cd(i);
//     hisM2PMxz[i-1] = new TH2F(hisNamexz.c_str(), ("his pm hits xz for omz " + omzp.str() + "m w.r.t water tank top; x (mm); z (mm)").c_str(), 100, -150, 150, 100, 0, 150);
//     M2DetectedEventTree->Draw(drawCommandxz.c_str(), drawCondition.c_str()); 
//     hisM2PMxz[i-1]->SetStats(0);
//     hisM2PMxz[i-1]->Draw("colz");
// } 

// TCanvas* cm4_pmxy = new TCanvas("cm4_pmxy", "Canvas PM M4 OM Z positions");
// cm4_pmxy->Divide(3,3);
// TH2F* hisM4PMxy[7];
// for(int i=1; i<8; i++){
//     std::ostringstream omzp;
//     double pos = -M4length/2. + i * M4length/8.; // minimum + i*step
//     omzp << -M4length/2. + pos;
//     // std::cout << "\r\033[10C" << "M4 position " << i << ": " << omzp.str() << std::endl;
//     std::string hisName=("hisM4PMxy_" + std::to_string(i)); 
//     // std::cout << "\r\033[10C" << "histName: " << hisName << std::endl;
//     std::string drawCommand="hits.pm_x:hits.pm_y >> hisM4PMxy_" + std::to_string(i);
//     // std::cout << "\r\033[10C" << "drawCommand: " << drawCommand << std::endl;
//     std::string drawCondition="OMPosition.data[2]/1000 > " + std::to_string(pos - 0.1) + " && OMPosition.data[2]/1000 < " + std::to_string(pos + 0.1);
//     std::cout << "\r\033[10C" << "drawCondition: " << drawCondition << std::endl;

//     cm4_pmxy->cd(i);
//     hisM4PMxy[i-1] = new TH2F(hisName.c_str(), ("his for omz " + omzp.str() + "m w.r.t water tank bottom; x (mm); y (mm)").c_str(), 100, -150, 150, 100, -150, 150);
//     M2DetectedEventTree->Draw(drawCommand.c_str(), drawCondition.c_str()); 
//     hisM4PMxy[i-1]->SetStats(0);
//     hisM4PMxy[i-1]->Draw("colz");
// } 

// TCanvas* cm6pm_xy = new TCanvas("cm6pm_xy", "Canvas PM M6 OM Z positions");
// cm6pm_xy->Divide(3,3);
// TH2F* hisM6PMxy[7];
// for(int i=1; i<8; i++){
//     std::ostringstream omzp;
//     double pos = -M6length/2. + i * M6length/8.; // minimum + i*step
//     omzp << -M6length/2. + pos;
//     std::string hisName=("hisM6PMxy_" + std::to_string(i)); 
//     std::string drawCommand="hits.pm_x:hits.pm_y >> hisM6PMxy_" + std::to_string(i);
//     std::string drawCondition="OMPosition.data[2]/1000 > " + std::to_string(pos - 0.1) + " && OMPosition.data[2]/1000 < " + std::to_string(pos + 0.1);
//     std::cout << "\r\033[10C" << "drawCondition: " << drawCondition << std::endl;

//     cm6pm_xy->cd(i);
//     hisM6PMxy[i-1] = new TH2F(hisName.c_str(), ("his for omz " + omzp.str() + "m w.r.t water tank bottom; x (mm); y (mm)").c_str(), 100, -150, 150, 100, -150, 150);
//     M2DetectedEventTree->Draw(drawCommand.c_str(), drawCondition.c_str()); 
//     hisM6PMxy[i-1]->SetStats(0);
//     hisM6PMxy[i-1]->Draw("colz");
// } 



// TCanvas* cM2_OM_xz = new TCanvas("cM2_OM_xz", "Canvas PM XZ M2 OM Z positions");
// cM2_OM_xz->Divide(3,3);
// TH2F* hisOM_M2_xz[7];

// for(int i=1; i<8; i++){
//     std::ostringstream omzp;
//     double pos = -M2length/2. + i * M2length/8.; // minimum + i*step
//     omzp << -M2length/2 + pos;;
//     std::string hisNamexz=("hisOM_M2_xz_" + std::to_string(i)); 
//     std::string drawCommandxz="hits.om_z:hits.om_x >> hisOM_M2_xz_" + std::to_string(i);
//     std::string drawCondition="OMPosition.data[2]/1000 == " + std::to_string(pos);

//     cM2_OM_xz->cd(i);
//     hisOM_M2_xz[i-1] = new TH2F(hisNamexz.c_str(), ("his OM hits XZ for omz " + omzp.str() + "m w.r.t water tank top; X (mm); Z (mm)").c_str(), 100, -220, 220, 100, -220, 220);
//     M2DetectedEventTree->Draw(drawCommandxz.c_str(), drawCondition.c_str()); 
//     hisOM_M2_xz[i-1]->SetStats(0);
//     hisOM_M2_xz[i-1]->Draw("colz");
// } 

// TCanvas* cm4_om_xz = new TCanvas("cm4_om_xz", "Canvas M4 OM Z positions");
// cm4_om_xz->Divide(3,3);
// TH2F* hisM4OMxz[7];
// for(int i=1; i<8; i++){
//     std::ostringstream omzp;
//     double pos = -M4length/2. + i * M4length/8.; // minimum + i*step
//     omzp << -M4length/2 + pos;;
//     // std::cout << "\r\033[10C" << "M4 position " << i << ": " << omzp.str() << std::endl;
//     std::string hisName=("hisM4OMxz_" + std::to_string(i)); 
//     // std::cout << "\r\033[10C" << "histName: " << hisName << std::endl;
//     std::string drawCommand="hits.om_z:hits.om_x >> hisM4OMxz_" + std::to_string(i);
//     // std::cout << "\r\033[10C" << "drawCommand: " << drawCommand << std::endl;
//     std::string drawCondition="OMPosition.data[2]/1000 > " + std::to_string(pos - 0.2) + " && OMPosition.data[2]/1000 < " + std::to_string(pos + 0.2);
//     std::cout << "\r\033[10C" << "drawCondition: " << drawCondition << std::endl;

//     cm4_om_xz->cd(i);
//     hisM4OMxz[i-1] = new TH2F(hisName.c_str(), ("his for omz " + omzp.str() + "m w.r.t water tank top; X (mm); Z (mm)").c_str(), 100, -220, 220, 100, -220, 220);
//     M2DetectedEventTree->Draw(drawCommand.c_str(), drawCondition.c_str()); 
//     hisM4OMxz[i-1]->SetStats(0);
//     hisM4OMxz[i-1]->Draw("colz");
// } 

// TCanvas* cm6_om_xz = new TCanvas("cm6_om_xz", "Canvas OM hits M6 OM Z positions");
// cm6_om_xz->Divide(3,3);
// TH2F* hisM6OMxz[7];
// for(int i=1; i<8; i++){
//     std::ostringstream omzp;
//     double pos = -M6length/2. + i * M6length/8.; // minimum + i*step
//     omzp << -M6length/2 + pos;
//     // std::cout << "\r\033[10C" << "M4 position " << i << ": " << omzp.str() << std::endl;
//     std::string hisName=("hisM6OMxz_" + std::to_string(i)); 
//     // std::cout << "\r\033[10C" << "histName: " << hisName << std::endl;
//     std::string drawCommand="hits.om_z:hits.om_x >> hisM6OMxz_" + std::to_string(i);
//     // std::cout << "\r\033[10C" << "drawCommand: " << drawCommand << std::endl;
//     std::string drawCondition="OMPosition.data[2]/1000 > " + std::to_string(pos - 0.2) + " && OMPosition.data[2]/1000 < " + std::to_string(pos + 0.2);
//     std::cout << "\r\033[10C" << "drawCondition: " << drawCondition << std::endl;

//     cm6_om_xz->cd(i);
//     hisM6OMxz[i-1] = new TH2F(hisName.c_str(), ("his for omz " + omzp.str() + "m w.r.t water tank top; X (mm); Z (mm)").c_str(), 100, -220, 220, 100, -220, 220);
//     M2DetectedEventTree->Draw(drawCommand.c_str(), drawCondition.c_str()); 
//     hisM6OMxz[i-1]->SetStats(0);
//     hisM6OMxz[i-1]->Draw("colz");
// } 


// // TCanvas* can2 = new TCanvas("can2", "Canvas");

// std::vector<pair<std::string, pair<int, TH1F*>>> detZenithHist;


// TH2F* mainM2Hist = new TH2F("mainM2Hist", "OM Z POZ VS Zenith distribution; om z pos; zenith",
//                             100, -2, 0, // om z position
//                             100, 0, 8 // zenith
//                         );

// TH2F* mainM4Hist = new TH2F("mainM4Hist", "OM Z POZ VS Zenith distribution; om z pos; zenith",
//                             100, -4, 0, // om z position
//                             100, 0, 8 // zenith
//                         );

// TH2F* mainM6Hist = new TH2F("mainM6Hist", "OM Z POZ VS Zenith distribution; om z pos; zenith",
//                             100, -6, 0, // om z position
//                             100, 0, 8 // zenith
//                         );

// TH3F* pmHits = new TH3F("pmHits", "pm_x; pm_y; pm_z",
//                             100, -200, 200, // om z position
//                             100, -200, 200, // zenith
//                             100, 0, 200 // zenith
//                         );

// gStyle->SetPalette(56);
// M2DetectedEventTree->Draw("hits.pm_z:hits.pm_y:hits.pm_x >> pmHits");
// TCanvas * c2 = new TCanvas("c1", "pmHits");
// pmHits->Draw("surf1z");

// TCanvas* can1 = new TCanvas("can1", "Zpostion");
// M2DetectedEventTree->Draw("hits.pm_x:hits.pm_y", " (OMPosition.data[2] == -750) * hits.pm_z", "colz");

// TCanvas * c3 = new TCanvas("c3", "zenDep.");


// for(int ze = 0; ze <= 60; ze+=2){
//     double zar = dtr(ze);
//     // std::cout << "\r\033[20C" << "ze deg: " << ze  << " -- computed zinith rad: " << zar << std::endl;
//     ostringstream angle;
//     angle << ze;
//     std::string histName = "M2_" + angle.str() + "_hist";
//     TH1F* hist = new TH1F(histName.c_str(), "Nhit distribution; nhits; Counts",
//                             100, 0, 1000
//                         );
    
//     TH2F* hist2d = new TH2F((histName + "_2D").c_str(), "Nhit distribution; nhits; Counts",
//                             100, 0, 1000,
//                             100, 0, 90
//                         );

//     // M2DetectedEventTree->Draw(("nhit >> " + histName).c_str(), (std::to_string(zar - 0.01) + " > zenith > " + std::to_string(zar + 0.01)).c_str());
//     detZenithHist.emplace_back("M2", make_pair(ze, hist));
// }


// int ih=0;
// for(auto& val : detZenithHist){
//     // TCanvas* c1 = new TCanvas(("c_z" + std::to_string(val.second.first)).c_str(), "canvas");
//     val.second.second->SetLineColor(ih+1);
//     if(ih == 1)
//         val.second.second->Draw();
//     else
//         val.second.second->Draw("same");

//     // c1->cd();
//     // val.second.second->Draw();
//     // std::cout << "\r\033[10C" << "hist name: " << val.second.second->GetName() << std::endl;
//     ih+=1;
// }

// TCanvas* can3 = new TCanvas("can3", "Canvas");

// // std::string pars = "zenith*180/3.1459:OMPosition.data[2]/1000 -1";
// M2DetectedEventTree->Draw("log10(event_energy):OMPosition.data[2]/1000 -1 >> mainM2Hist");
// M4DetectedEventTree->Draw("log10(event_energy):OMPosition.data[2]/1000 -2 >> mainM4Hist");
// M6DetectedEventTree->Draw("log10(event_energy):OMPosition.data[2]/1000 -3 >> mainM6Hist");


// TCanvas* c1 = new TCanvas("testc ", "Energy as funstion of OM position");
// c1->Divide(2,2);
// c1->cd(1);
// mainM2Hist->Draw("surf2z");
// gPad->SetTheta(30);  // Elevation
// gPad->SetPhi(165);   // Azimuth

// c1->cd(2);
// mainM4Hist->Draw("surf2z");
// gPad->SetTheta(30);  // Elevation
// gPad->SetPhi(165);   // Azimuth

// c1->cd(3);
// mainM6Hist->Draw("surf2z");
// gPad->SetTheta(30);  // Elevation
// gPad->SetPhi(165);   // Azimuth


}
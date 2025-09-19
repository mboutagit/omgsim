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


// int main(int arg_v, char** agr_v){

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
M2DetectedEventTree->Draw("nhit", "", "");
M4DetectedEventTree->Draw("nhit", "", "same");
M6DetectedEventTree->Draw("nhit", "", "same");

std::vector<pair<std::string, pair<int, TH1F*>>> detZenithHist;


TH2F* mainM2Hist = new TH2F("mainM2Hist", "OM Z POZ VS Zenith distribution; om z pos; zenith",
                            100, -2, 0, // om z position
                            100, 0/180. * TMath::Pi(), 60/180. * TMath::Pi() // zenith
                        );

TH2F* mainM4Hist = new TH2F("mainM4Hist", "OM Z POZ VS Zenith distribution; om z pos; zenith",
                            100, -4, 0, // om z position
                            100, 0/180. * TMath::Pi(), 60/180. * TMath::Pi() // zenith
                        );

TH2F* mainM6Hist = new TH2F("mainM6Hist", "OM Z POZ VS Zenith distribution; om z pos; zenith",
                            100, -6, 0, // om z position
                            100, 0/180. * TMath::Pi(), 60/180. * TMath::Pi() // zenith
                        );

for(int ze = 0; ze <= 60; ze+=2){
    double zar = ze/180. * TMath::Pi();
    std::cout << "\r\033[20C" << "Computed za rad: " << zar  << ("  || zenith == " + std::to_string(zar)).c_str()<< std::endl;
    std::string histName = "M2_" + std::to_string(ze) + "_hist";
    TH1F* hist = new TH1F(histName.c_str(), "Nhit distribution; nhits; Counts",
                            100, 0, 1000
                        );
    
    TH2F* hist2d = new TH2F((histName + "_2D").c_str(), "Nhit distribution; nhits; Counts",
                            100, 0, 1000,
                            100, 0, 90
                        );

    M2DetectedEventTree->Draw(("nhit >> " + histName).c_str(), (std::to_string(zar - 0.01) + " > zenith > " + std::to_string(zar + 0.01)).c_str());
    detZenithHist.emplace_back("M2", make_pair(ze, hist));
}


int ih=0;
for(auto& val : detZenithHist){
    // TCanvas* c1 = new TCanvas(("c_z" + std::to_string(val.second.first)).c_str(), "canvas");
    val.second.second->SetLineColor(ih+1);
    if(ih == 1)
        val.second.second->Draw();
    else
        val.second.second->Draw("same");

    // c1->cd();
    // val.second.second->Draw();
    // std::cout << "\r\033[10C" << "hist name: " << val.second.second->GetName() << std::endl;
    ih+=1;
}

M2DetectedEventTree->Draw("zenith:OMPosition.data[2]/1000 -1 >> mainM2Hist");
M4DetectedEventTree->Draw("zenith:OMPosition.data[2]/1000 -2 >> mainM4Hist");
M6DetectedEventTree->Draw("zenith:OMPosition.data[2]/1000 -3 >> mainM6Hist");


TCanvas* c1 = new TCanvas("testc ", "canvas");
c1->Divide(2,2);
c1->cd(1);
mainM2Hist->Draw("surf2z");
gPad->SetTheta(30);  // Elevation
gPad->SetPhi(165);   // Azimuth

c1->cd(2);
mainM4Hist->Draw("surf2z");
gPad->SetTheta(30);  // Elevation
gPad->SetPhi(165);   // Azimuth

c1->cd(3);
mainM6Hist->Draw("surf2z");
gPad->SetTheta(30);  // Elevation
gPad->SetPhi(165);   // Azimuth


}
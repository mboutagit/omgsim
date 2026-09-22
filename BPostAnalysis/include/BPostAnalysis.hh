#ifndef __BPOSTANALYSIS_HH__
#define __BPOSTANALYSIS_HH__

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TKey.h>
#include <TLeaf.h>
#include <TCollection.h> 
#include <TCanvas.h> 
#include <TH1.h> 
#include <TH2.h> 
#include <TH3.h> 
#include <TApplication.h>
#include <TPolyLine3D.h>
#include <TArrow.h>
#include <TLine.h>
#include <TView.h>
#include <TBox.h>
#include <TLegend.h>
#include "TFrame.h"
#include "TGrid.h"

#include "TROOT.h"
#include "TGraphAsymmErrors.h"
// #include "TIter.h"

#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>

#include <TSystem.h>  // For gSystem->ProcessEvents()
#include <unistd.h>   // For usleep()

#include "BAnalysisManager.hh"
#include "BLactelDetector.hh"
#include "BDetectorConstruction.hh"

#include <H5Cpp.h>
#include <variant>
#include "utils.hh"
#include "TGaxis.h"

#include <map>


using namespace UTILS;
namespace lactel{

    typedef unsigned int uint;

    struct PhotonProduction
    {   
        uint run_id;
        uint evt_id;
        float VertexX=0;
        float VertexY=0;
        float VertexZ=0;
        float VertexT=0;
        float VertexP=0;
        float VertexDirX=0;
        float VertexDirY=0;
        float VertexDirZ=0;
        float VertexEnergy=0;
        float PhotoX=0;
        float PhotoY=0;
        float PhotoZ=0;
        float PhotoT=0;
        float PhotoP=0;
        short OMId=-1;
        short PMID=0;
    };

    struct Hit{

        float fTotalTime=0;
        float fEnergy=0;
        bool fPolarity =0;
        unsigned char fLimit=0;
    };

    struct PhotonArrival
    {
        float OMHitX=0;
        float OMHitY=0;
        float OMHitZ=0;
        float OMHitT=0;
        float OMHitP=0;
    };


    typedef  std::vector<std::pair<std::string, TH1*>> histsByName;

    class BPostAnalysis
    {
    private:
        /* data */
    public:
        BPostAnalysis(/* args */);
        ~BPostAnalysis();

        std::map<std::string, TFile*> inputRootFiles;
        std::map<std::string, TH1*> Hists1D;
        std::map<std::string, TH2*> Hists2D;
        std::map<std::string, TGraph*> TGraphs;

        std::map<std::string, std::vector<std::string>> files;
        // TH* getRealDataPlots(std::string file1)

        inline static constexpr int mVperBin = 20;
        static BLactelDetector detector;


    };

    struct WaterTank {
        int id;
        float xo;
        float yo;
        float zo;
        float xw;
        float yw;
        float zw;
        WaterTank(int fid, float fxo, float fyo, float fzo, float fxw, float fyw, float fzw){
            id = fid;
            xo = fxo;
            yo = fyo;
            zo = fzo;
            xw = fxw;
            yw = fyw;
            zw = fzw;
        }
    };
    
    

    std::vector<TCanvas*>  getNPEPlot(TTree* tree,
                                        histsByName& hist, 
                                        float exposure_time = 1, float t_coin = 16, float thr = 0); // in second
    TCanvas* getDAQParameter(TTree* tree, BAnalysisManager::DetectedEvent* evt, std::string par); // in mV
    TCanvas* getVertexPositions(TTree* tree);
    std::vector<TCanvas*>  getRealDataPlots(histsByName& hist, std::string file1, std::string file2); 
    void  getRealDataByInFile(histsByName& hists, std::string inFile, std::string dataset, 
                                            float xmin, float xmax, float unitsPerBin, float attenuator = 1.); 

    TCanvas* makeRatioPlot(TH1* h1, TH1* h2);

    using stack = std::variant<
                                std::map<std::string, TH1F*>,
                                std::map<std::string, TH2F*>,
                                std::map<std::string, TH3F*>,
                                std::map<std::string, TGraph*>
                            >;
    
    struct Container {
        std::map<std::string, TH1F*> h1;
        std::map<std::string, TH2F*> h2;
        std::map<std::string, TH3F*> h3;
        std::map<std::string, TGraph*> graphs;
    };


    TGraph* ratio(TH1*, TH1*);
    TH1* getHistByName(const histsByName& vec, std::string key);

    TCanvas* DrawWithRatio(TH1* h1, TH1* h2, const char* canvasName = "cRatio");
    TCanvas* DrawWithRatio(std::vector<std::pair<TH1*, TH1*>> hists, const char* canvasName = "cRatio", bool errorBand = true);
    // TH1* getHistByName(histsByName container, std::string hist_name);


} // namespace lactel
#endif
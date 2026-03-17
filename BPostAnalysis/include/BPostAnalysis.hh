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
#include <TH1F.h> 
#include <TH2F.h> 
#include <TH3F.h> 
#include <TApplication.h>
#include <TPolyLine3D.h>
#include <TArrow.h>
#include <TLine.h>
#include <TView.h>
#include <TBox.h>
#include <TLegend.h>

#include "TROOT.h"
// #include "TIter.h"

#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>

#include <TSystem.h>  // For gSystem->ProcessEvents()
#include <unistd.h>   // For usleep()

#include "BAnalysisManager.hh"

#include <H5Cpp.h>
#include <variant>


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

    };
    

    std::vector<TCanvas*>  getNPEPlot(TTree* tree, BAnalysisManager::DetectedEvent* evt, 
                                        histsByName& hist, 
                                        float exposure_time = 1, float t_coin = 16); // in second
    TCanvas* getDAQParameter(TTree* tree, BAnalysisManager::DetectedEvent* evt, std::string par); // in mV
    std::vector<TCanvas*>  getRealDataPlots(histsByName& hist); 

    using stack = std::variant<
                                std::pair<std::string, TH1F*>,
                                std::pair<std::string, TH2F*>,
                                std::pair<std::string, TH3F*>,
                                std::pair<std::string, TGraph*>
                            >;
    
    std::vector<stack> container;

    TGraph* ratio(TH1*, TH1*);
    TH1* getHistByName(const histsByName& vec, std::string key);

} // namespace lactel
#endif
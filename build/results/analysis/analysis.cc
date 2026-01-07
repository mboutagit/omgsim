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


typedef unsigned int uint;

struct PhotonProducion
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

struct GenPars
{
    int GenPPDGID;
    unsigned int GenEvtID;
    float GenXPos;
    float GenYPos;
    float GenZPos;
    float GenXDir;
    float GenYDir;
    float GenZDir;
    float GenTheta;
    float GenPhi;
    float GenEnergy;
};


struct Event
{                    
    int  event_id = -1;
    int runNb = -1;
    uint nhit=0;
    float tota=0.;
    float tmin=0.;
    float tmax=0.;

    bool isDetected=false; // mainly used for Run managing
};

struct Run {
    int runNb = -1;
    uint nGenEvent = 0;
    uint nDetEvent = 0;
};


class treeName
{
private:
    /* data */
public:
    treeName(/* args */);
    treeName(TFile* file);
    ~treeName();
    bool readRootFile(TFile* file);
    std::map<std::string, std::vector<std::string>> treeBranches;
    std::map<TTree*, std::vector<TBranch*>> tree_branch;
};

treeName::treeName(){};
treeName::treeName(TFile *file){
    // std::vector<TTree*> tree_name;
    // std::vector<TBranch*> branch_name;

    // if (!file || file->IsZombie()) {
    //     std::cerr << "Error: Cannot open file!" << std::endl;
    //     return false;
    // }

    // TIter next(file->GetListOfKeys());
    // TKey *key;
    // while ((key = (TKey*)next())) {
    //     if (std::string(key->GetClassName()) == "TTree") {
    //         TTree *tree = (TTree*)file->Get(key->GetName());
    //         tree_name.push_back()
    //         if (!tree) {
    //             file->Close();
    //             return false;
    //         }           
            
    //         branch_name.clear();
    //         TIter nextBranch(tree->GetListOfBranches());
    //         TBranch *branch;
    //         while ((branch = (TBranch*)nextBranch())) {
    //             branch_name.push_back(branch->GetName());
    //         }
    //         treeBranches.insert(std::pair<, std::vector<std::string>>(key->GetName(), branch_name));
    //     }
    // }
};

bool treeName::readRootFile(TFile* file)
{
    std::vector<std::string> branch_name;

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open file!" << std::endl;
        return false;
    }

    TIter next(file->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
        if (std::string(key->GetClassName()) == "TTree") {
            TTree *tree = (TTree*)file->Get(key->GetName());
            if (!tree) {
                file->Close();
                return false;
            }           
            
            branch_name.clear();
            TIter nextBranch(tree->GetListOfBranches());
            TBranch *branch;
            while ((branch = (TBranch*)nextBranch())) {
                branch_name.push_back(branch->GetName());
                TObjArray *leaves = branch->GetListOfLeaves();
            
                std::cout << "Leaves in branch '" << branch->GetName() << "':" << std::endl;
                for (int i = 0; i < leaves->GetEntries(); ++i) {
                    TLeaf *leaf = (TLeaf*)leaves->At(i);
                    std::cout << "  Name: " << leaf->GetName() 
                          << ", Type: " << leaf->GetTypeName() << std::endl;
                }
            }
            treeBranches.insert(std::pair<std::string, std::vector<std::string>>(key->GetName(), branch_name));
        }
    }

    return true;
}

treeName::~treeName()
{
}



using namespace std;

int main(int argc, char* argv[]){

    TApplication app("ROOTApp", &argc, argv);
    gSystem->Load("../../libBAnalysis.so");
    gSystem->Load("../../liblibOMG.so");
    
    std::cout << "argc = " << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }

    if(argc < 2){
    cout << "Syntax error: expected root file name" << endl;
    return 1;
    }

    string rootfilename = "../../default.root";

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

    GenPars gps;
    Event evt;
    PhotonProducion pp;
    Hit hit;

    // TBranch *gpsbr = genTree->GetBranch("gen");
    // gpsbr->SetAddress(&gps);
    // gpsbr->SetAutoDelete(kTRUE);

    // TBranch *evtbr = genTree->GetBranch("Event");
    // evtbr->SetAddress(&evt);
    // evtbr->SetAutoDelete(kTRUE);

    genTree->SetBranchAddress("gen", &gps);
    genTree->SetBranchAddress("Event", &evt);

    TBranch *ppbr = hitTree->GetBranch("Vertex");
    ppbr->SetAddress(&pp);
    ppbr->SetAutoDelete(kTRUE);

    TBranch *hitbr = hitTree->GetBranch("Hit");
    hitbr->SetAddress(&hit);
    hitbr->SetAutoDelete(kTRUE);

    TCanvas *c = new TCanvas("c", "Arrow Example", 800, 600);

    TH3F *dir = new TH3F("dir","Dir; X; Y; Z",  300, -8000, 8000,
                                                300, -8000, 8000,
                                                300, -3000, 3000);

    //genTree->Draw("Gen.GenZPos:Gen.GenYPos:Gen.GenXPos", "","");

    std::vector<TH2F*> EvsTheta;
    std::vector<TH1F*> E_theta;
    std::vector<float> angles = {90, 100, 110, 120, 130, 140, 150, 160, 170, 180};


    
    for (uint i = 0; i < angles.size() -1 ; i++)
    {
       E_theta.push_back(new TH1F(  ("his" + to_string(i)).c_str(), 
                                    ("Energy vs #theta - [" + to_string((int) angles[i]) + " - "+ to_string((int)angles[i+1]) + "]; Energy (MeV); Events").c_str(), 
                                    100, 90, 180
                                )
                        );
    }
    


    //dir->Draw();
    //uint color = 0;
    cout << "\r\033[20C" << "genTree Entries: " << genTree->GetEntries() << endl;
    for(Int_t i=0; i<= genTree->GetEntries(); i++){
        genTree->GetEntry(i);

        cout << "Event is: " << evt.event_id 
                << " -- runnb: " << evt.runNb
                << " -- isDet: " << evt.isDetected 
                << " -- nhit: " << evt.nhit  
                << " -- tota: " << evt.tota 
        << endl;
        
        // for (uint j = 0; j < angles.size() - 1; j++)
        // {
        //     if(gps.GenTheta <= angles[j+1])
        //         E_theta[j]->Fill(gps.GenTheta);
        //        // cout << "\r\033[20C" << "energy - " << gps.GenEnergy  << endl;
        // }
        
        // float angle = 
       // cout << "\r\033[20C" << "gen theta: " << gps.GenTheta << endl;

        // double x[] = {gps.GenXPos, gps.GenXPos + 3000 * gps.GenXDir};  
        // double y[] = {gps.GenYPos, gps.GenYPos + 3000 * gps.GenYDir};  
        // double z[] = {gps.GenZPos, gps.GenZPos + 3000 * gps.GenZDir};  
        
        // uint maxPoints = 200;
        // if(gps.GenTheta > 170)
        // for (uint j = 0; j < maxPoints; j++)
        // {
        //     dir->Fill(gps.GenXPos + 4*j * gps.GenXDir, 
        //                 gps.GenYPos + 4*j * gps.GenYDir, 
        //                 gps.GenZPos + 4*j * gps.GenZDir);
            
        //     // dir->Draw("same");
        //     // gSystem->ProcessEvents();
        //     // c->Update();
        //     // usleep(50);
        // }
    }
    
   // E_theta[0]->Draw();
//    uint k=0;
//     for(auto& his : E_theta){
//         //if(his)
//             cout << "\r\033[20C" << "hist name: " << his->GetName() << endl;
//             his->SetLineColor(angles.size()-k-1);
//             his->SetStats(0);
//             if(k==0)
//                 his->GetYaxis()->SetRangeUser(0, 200);
//             his->Draw("same");
//             k+=1;
//     }

    // TCanvas *c1 = new TCanvas("c1", "Arrow Example", 800, 600);
    // E_theta[0]->Draw();
    // TCanvas *c2 = new TCanvas("c2", "Arrow Example", 800, 600);
    // E_theta[1]->Draw();
    // TCanvas *c3 = new TCanvas("c3", "Arrow Example", 800, 600);
    // E_theta[2]->Draw();




    //hitTree->Draw("1240 / (Hit.Energy * 1e6)", "Vertex.PhotoT < 0.7","");

    app.Run();
    return 0;
}
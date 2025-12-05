
#ifndef BAnalysisManager_h 
#define BAnalysisManager_h 

#include "AAAnalysisManager.hh"
#include "globals.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4Run.hh"

#include "TF1.h"
#include "TGraph.h"

#include "BEvent.hh"
#include "OM.hh"

using namespace std;
using namespace km3net;

namespace lactel
{
    class BRunAction;

    class BAnalysisManager : public AAAnalysisManager
    {
        public:
            struct HitInfos
            {
                int hit_id;
                double px; // pos. X
                double py;
                double pz;
                double dt; // dir. Theta 
                double dp; // dir. Phi 
            };

            struct GenerationParameters
            {
                int fGenPPDGID = -1;
                unsigned int fGenEvtID = -1;
                float fGenXPos = -1;
                float fGenYPos = -1;
                float fGenZPos = -1;
                float fGenXDir = -1;
                float fGenYDir = -1;
                float fGenZDir = -1;
                float fGenTheta = -1;
                float fGenPhi = -1;
                float fGenEnergy = -1;
                bool isDetected = false;

                // std::vector<HitInfos> omHits;
                // std::vector<HitInfos> pmHits;
            };
            
            class MeasureParameters
            {
                private:
                    // static float HV;
                    // static std::map<float, float> quantum_efficiency;
                    TF1* gain_vs_highVoltage = nullptr;
                    TF1* muon_flux = nullptr;
                    TGraph* QEVsLambda_graph = nullptr;

                public:
                    MeasureParameters();
                    virtual ~MeasureParameters();

                    inline float getGain(float hv){return exp(-0.93 + 0.01*hv);}
                    static inline double getGainROOT(double* x, double* params) {
                        return exp(-0.93 + 0.01*x[0]);
                    }
                    float getQE(float lambda);
                    TF1* getGainVsHV();
                    TF1* getMuonFlux();
                    TGraph* getQEVsLambda();    
                    
                    ClassDef(MeasureParameters, 1); 
            };

            class Measure
            {
                private:
                    MeasureParameters fMeasure_Pars;
                    unsigned int fId;
                    float fEnergy;
                    float fNpe;
                    float fTime;

                    friend class BAnalysisManager;
                public:
                    Measure();
                    Measure(float);
                    virtual ~Measure();
                    void print(std::ostream&);
                    float getEnergy() const{ return fEnergy;}
                    unsigned int getID() const{return fId;}

                ClassDef(Measure, 1);
            };

            struct Hit;
            struct DetectedEvent
            {                    
                // unsigned int event_ID;
                std::vector<Hit> hits;
                // std::vector<HitInfos> pmHits;
                // std::vector<HitInfos> hits;
                float event_energy;
                int  event_id = -1;
                int runNb = -1;
                uint nhit=0;
                float tota=0.; // cumulative charge
                float a0 = 0; 
                float a1 = 0; 
                float tmin=0.;
                float tmax=0.;
                float zenith;
                G4ThreeVector OMPosition;

                // bool isDetected=false; // mainly used for Run managing

            //     friend class BAnalysisManager;
            // public:
            //     Event(){};
            //     ~Event(){};
            };

            struct Hit{
                long id = 0;
                float time = 0;
                float charge = 0;
                float a = 0;
                float a_f = 0;
                float t_tts = 0;
                
                float pm_x; 
                float pm_y; 
                float pm_z; 
                float pm_t; 
                float pm_p; 

                float om_x; 
                float om_y; 
                float om_z; 
                float om_t; 
                float om_p; 

                int om_id;
            };

            struct Run {
                int runNb = -1;
                uint nGenEvent = 0;
                uint nDetEvent = 0;
                float expectedRate = 0;
                float generationSurface = 0; // in cm²
                float acceptanceAngle = -999; // in deg

                std::string detName ="";
            };
              
            MeasureParameters fMeasure_parameters;
            Measure fMeasure;
            // std::map<unsigned int, Event> fEvent_map;
            // std::multimap<unsigned int, Measure> eventMeasure_map;

            MeasureParameters getMeasurePars(); 
            Measure getMeasure();
            DetectedEvent fDetectedEvent;
            Run fRun;
            
        protected:
            BAnalysisManager();
            ~BAnalysisManager();
            TTree* genTree;
            TTree* runTree;
            TTree* detectedEventTree;
            GenerationParameters genPars;

            int ndet = 0;
            int nsim = 0;
            int nhit = 0;

            Hit fHits;

            friend class BRunAction;

        public:
            static BAnalysisManager* getIt();
            virtual void Write();
            virtual void AddDetectedParticule(const KM3TrackInformation* info, unsigned char LIM);
            void AddPhotoTubeEntranceInfo (const G4Step* STEP);
            void AddOMEntranceInfo (const G4Step* STEP);

           // virtual void AddPhotoTubeEntranceInfo (const G4Step* fStep);
            // virtual void AddOMEntranceInfo (const G4Step* Step);
            virtual void EndOfEvent(const G4Event* evt);
            virtual void StartOfEvent(const G4Event*);
            virtual void Init(std::string rfn = "");
            inline Hit& getHit(){return fHits;}

            std::string evtFileName = "output.evt";
            

        ClassDef(BAnalysisManager, 1);

        
    };

    // struct Event
    // {
    //     uint event_id=0;
    //     uint nhit=0;
    //     float tota=0.;
    //     float tmin=0.;
    //     float tmax=0.;
    // };
    
    
} // namespace lactel

#endif
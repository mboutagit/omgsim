
#ifndef BAnalysis_h 
#define BAnalysis_h 

#include "AAAnalysisManager.hh"
#include "globals.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4Run.hh"

#include "TF1.h"
#include "TGraph.h"

using namespace std;
using namespace km3net;

namespace lactel
{
    class BRunAction;

    class BAnalysis : public AAAnalysisManager
    {
        public:

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

                    friend class BAnalysis;
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
                    float event_energy;
                    std::vector<Hit> hits;
                    int  event_id = -1;
                    int runNb = -1;
                    uint nhit=0;
                    float tota=0.; // cumulative charge
                   //float a = 0; // hit charge
                    float tmin=0.;
                    float tmax=0.;

                   // bool isDetected=false; // mainly used for Run managing

                //     friend class BAnalysis;
                // public:
                //     Event(){};
                //     ~Event(){};
            };

            struct Hit{
                long id = 0;
                float time = 0;
                float charge = 0;
            };

            struct Run {
                int runNb = -1;
                uint nGenEvent = 0;
                uint nDetEvent = 0;
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
            BAnalysis();
            ~BAnalysis();

            virtual void Write();
            virtual void AddDetectedParticule(const KM3TrackInformation* info, unsigned char LIM);
            // virtual void AddPhotoTubeEntranceInfo (const G4Step* fStep);
            // virtual void AddOMEntranceInfo (const G4Step* Step);

            TTree* genTree;
            TTree* runTree;
            TTree* detectedEventTree;
            GenerationParameters genPars;

            friend class BRunAction;

        public:
            static BAnalysis* getIt();
            virtual void EndOfEvent(const G4Event* evt);
            virtual void StartOfEvent(const G4Event*);
            virtual void Init(std::string rfn = "");



        ClassDef(BAnalysis, 1);

        
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

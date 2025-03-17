
#include "AAAnalysisManager.hh"
#include "globals.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4Run.hh"
#include "KM3RunAction.hh"

#include "TF1.h"
#include "TGraph.h"

using namespace std;
namespace km3net
{
    
    class BAnalysis : public AAAnalysisManager, public KM3RunAction
    {
        public:

            struct GenerationParameters
            {
                int fGenPPDGID;
                unsigned int fGenEvtID;
                float fGenXPos;
                float fGenYPos;
                float fGenZPos;
                float fGenXDir;
                float fGenYDir;
                float fGenZDir;
                float fGenTheta;
                float fGenPhi;
                float fGenEnergy;
            };
            
            class MeasureParameters
            {
                private:
                    static float HV;
                    static std::map<float, float> quantum_efficiency;
                    TF1* gain_vs_highVoltage = nullptr;
                   TF1* muon_flux = nullptr;
                    TGraph* QEVsLambda_graph = nullptr;

                public:
                    MeasureParameters();
                    ~MeasureParameters();

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
                    ~Measure();
                    void print(std::ostream&);
                    float getEnergy() const{ return fEnergy;}
                    unsigned int getID() const{return fId;}

                ClassDef(Measure, 1);
            };

            struct Event
            {                    
                    // unsigned int event_ID;
                    // float event_energy;
                    int  event_id = -1;
                    int runNb = -1;
                    uint nhit=0;
                    float tota=0.;
                    float tmin=0.;
                    float tmax=0.;

                    bool isDetected=false; // mainly used for Run managing

                //     friend class BAnalysis;
                // public:
                //     Event(){};
                //     ~Event(){};
            };

            struct Run {
                int runNb = -1;
                uint nGenEvent = 0;
                uint nDetEvent = 0;
            };
              
            MeasureParameters fMeasure_parameters;
            Measure fMeasure;
            std::map<unsigned int, Event> fEvent_map;
            std::multimap<unsigned int, Measure> eventMeasure_map;

            MeasureParameters getMeasurePars(); 
            Measure getMeasure();
            Event fEvent;
            Run fRun;
            
        protected:
            BAnalysis();
            ~BAnalysis();


            virtual void EndOfEvent();
            virtual void StartOfEvent(const G4Event*);
            virtual void Write();
            virtual void Init(std::string FR = "");
            virtual void AddDetectedParticule(const KM3TrackInformation* info, unsigned char LIM);

            void BeginOfRunAction(const G4Run*);
            void EndOfRunAction(const G4Run*);

            TTree* genTree;
            TTree* runTree;
            TTree* bEventTree;
            GenerationParameters genPars;

        public:
            static BAnalysis* getIt();

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
    
    
} // namespace km3net

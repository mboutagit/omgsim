
#include "BEventAction.hh"
#include "G4DigiManager.hh"
#include "BWaveCatcher.hh"
#include "BRunManager.hh"
#include "BDetectorConstruction.hh"
# include "BLogger.hh"

#include "G4SystemOfUnits.hh"



using namespace km3net;
using namespace tools;
using namespace utils;

namespace lactel
{
    BEventAction::BEventAction(){};
    BEventAction::~BEventAction(){};

    double BEventAction::globalTime = 0. *ns;

    void BEventAction::BeginOfEventAction(const G4Event* anEvent){
        G4double muonRate = 500. * hertz;  // <-- YOUR computed rate
        G4double dt = -std::log(G4UniformRand()) / muonRate;
        globalTime += dt;

        if(! BDetectorConstruction::GetIt()->IsRunning()){
            BLOG_DEBUG("Detector is not running");
            EndOfEventAction(anEvent);
            return;
        }
        KM3EventAction::BeginOfEventAction(anEvent);
        BAnalysisManager::getIt()->StartOfEvent(anEvent);
    }
    
    void BEventAction::EndOfEventAction(const G4Event* anEvent){
        G4int event_id       = anEvent->GetEventID();
        KM3EventAction::EndOfEventAction (anEvent);

        BAnalysisManager* ana = BAnalysisManager::getIt();
        ana->EndOfEvent(anEvent);   // stop recording
    }

} // namespace lactel

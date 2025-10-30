
#include "BEventAction.hh"
#include "G4DigiManager.hh"
#include "BWaveCatcher.hh"

using namespace km3net;

namespace lactel
{
    BEventAction::BEventAction(){};
    BEventAction::~BEventAction(){};

    void BEventAction::BeginOfEventAction(const G4Event* anEvent){
        KM3EventAction::BeginOfEventAction(anEvent);
        BAnalysis::getIt()->StartOfEvent(anEvent);
    
    }
    
    void BEventAction::EndOfEventAction(const G4Event* anEvent){
        G4int event_id       = anEvent->GetEventID();
        //analysis
        KM3EventAction::EndOfEventAction (anEvent);
        BAnalysis::getIt()->EndOfEvent(anEvent);  
        //Digi
        // G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
        // digiMan->Digitize("DigitizerModule");  
        // BWaveCatcher* wc1 = new BWaveCatcher();
        // Waveform wf = wc1->getFakeWaveform(1); // get channel one wavefrom

        // wc1->drawWaveform(wf).Write();


    }

} // namespace lactel


#include "BEventAction.hh"
#include "G4DigiManager.hh"
#include "BWaveCatcher.hh"
#include "BRunManager.hh"

using namespace km3net;
using namespace tools;

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

        BAnalysis* ana = BAnalysis::getIt();
        // for(auto& hit : ana->fDetectedEvent.hits){
        //     // std::cout << "\r\033[10C" << "hit charge: " << hit.charge << std::endl;
        // }

        //Digi
        // G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
        // digiMan->Digitize("DigitizerModule");  
        // BWaveCatcher* wc1 = new BWaveCatcher();
        // wc1->startAcquisition();
        // wc1->getConfiguration().disableAllChannels();
        // // Waveform wf = wc1->getFakeWaveform(1); // get channel one wavefrom
        // Waveform wf = wc1->digitizeEvent(1, ana->getDetectedEvent()); // get channel one wavefrom
        // wc1->drawWaveform(wf).Write();
        ana->EndOfEvent(anEvent);   // stop recording

    }

} // namespace lactel

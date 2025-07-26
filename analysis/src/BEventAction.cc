
#include "BEventAction.hh"
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
        }


    
} // namespace lactel

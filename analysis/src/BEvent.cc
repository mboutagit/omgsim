
#include "BEvent.hh"

namespace lactel
{
    BEvent::BEvent(){}
    BEvent::BEvent(G4int evt_id) : G4Event(evt_id) {}

    BEvent::~BEvent(){}

    std::vector<BHit> BEvent::getHitCollection(){return hitCollection;}
    
    void BEvent::addHit(BHit ahit){
        hitCollection.push_back(ahit);
    }
    
} // namespace lactel

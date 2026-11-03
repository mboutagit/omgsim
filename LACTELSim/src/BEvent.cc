
#include "BEvent.hh"

namespace lactel
{   
    BEvent::BEvent(G4int eventID) : G4Event(eventID) {}
    BEvent::~BEvent(){}

    std::vector<BHit> BEvent::getHitCollection(){return hitCollection;}
    
    void BEvent::addHit(BHit ahit){
        hitCollection.push_back(ahit);
    }
    
} // namespace lactel

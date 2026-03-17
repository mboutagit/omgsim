#ifndef BStachingaction_hh 
#define BStachingaction_hh 

#include "G4UserStackingAction.hh"
#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "BDetectorConstruction.hh"

using namespace lactel; 

class BStackingAction : public G4UserStackingAction {
public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track);
};

#endif
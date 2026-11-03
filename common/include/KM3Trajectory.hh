#ifndef KM3Trajectory_h
#define KM3Trajectory_h

#include "G4VTrajectory.hh"
#include "G4Trajectory.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include <vector>

class KM3Trajectory : public G4Trajectory {
public:
    KM3Trajectory(const G4Track* track);
    ~KM3Trajectory() override;

    void DrawTrajectory() const override;
    // Other overridden methods if required...
    
private:
    G4Trajectory* fTrajectory;
    G4String fParticleName;
    //std::vector<G4ThreeVector> fTrajectoryPoints; // Store trajectory points
};

#endif
#ifndef KM3Trajectory_h
#define KM3Trajectory_h

#include "G4VTrajectory.hh"
#include "G4Trajectory.hh"
#include "G4VisManager.hh"
#include "G4Track.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

using namespace std;

class KM3Trajectory : public G4VTrajectory {
public:
    KM3Trajectory(const G4Track*);
    virtual ~KM3Trajectory();

    virtual void DrawTrajectory() const override;

    // Other necessary methods...
    virtual G4VTrajectory* Clone() const override;
};

#endif
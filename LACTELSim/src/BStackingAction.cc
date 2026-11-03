#include "BStackingAction.hh"

 G4ClassificationOfNewTrack BStackingAction::ClassifyNewTrack(const G4Track* track)
    {

        if (track->GetDefinition() == G4OpticalPhoton::Definition())
        {
            G4LogicalVolume* target = BDetectorConstruction::GetIt()->getPhysicalVolumeByName("LacWater")->GetLogicalVolume();
            // if(target)
            //     if (track->GetLogicalVolumeAtVertex() == target)
                    // return fKill;   // killed before tracking starts
        }
        return fUrgent;
    }
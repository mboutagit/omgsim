#include "KM3TrackInformation.hh"
#include "G4ios.hh"
#include <G4VPhysicalVolume.hh>
#include <G4NavigationHistory.hh>

G4Allocator<KM3TrackInformation> aTrackInformationAllocator;

KM3TrackInformation::KM3TrackInformation()
{
  originalTrackID = 0;
  particleDefinition = 0;
  originalPosition = G4ThreeVector(0.,0.,0.);
  originalMomentum = G4ThreeVector(0.,0.,0.);
  originalEnergy = 0.;
  originalTime = 0.;
}

KM3TrackInformation::KM3TrackInformation(const G4Track* aTrack)
{
  originalTrackID = aTrack->GetTrackID();
  particleDefinition = aTrack->GetDefinition();
  originalPosition = aTrack->GetPosition();
  originalLocalPosition =
    aTrack->GetStep ()->GetPostStepPoint ()->
    GetTouchable()->GetHistory ()->GetTopTransform().TransformPoint(originalPosition);
  originalMomentum = aTrack->GetMomentum();
  originalEnergy = aTrack->GetTotalEnergy();
  originalTime = aTrack->GetGlobalTime();

  originalPolarization = aTrack->GetPolarization ();

  originalPreVolume = aTrack->GetStep ()->GetPreStepPoint()->GetPhysicalVolume();
  originalPostVolume = aTrack->GetStep ()->GetPostStepPoint()->GetPhysicalVolume();
}

KM3TrackInformation::~KM3TrackInformation(){;}

void KM3TrackInformation::Print() const
{
  G4cout
    << "name " << particleDefinition->GetParticleName () << " Original track ID " << originalTrackID
    << " at " << originalPosition << G4endl;
}

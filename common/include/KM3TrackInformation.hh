#ifndef KM3TrackInformation_h
#define KM3TrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class G4VPhysicalVolume;

class KM3TrackInformation : public G4VUserTrackInformation
{
public:
  KM3TrackInformation();
  KM3TrackInformation(const G4Track* aTrack);
  virtual ~KM3TrackInformation();

  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);
  inline int operator ==(const KM3TrackInformation& right) const
  {return (this==&right);}

  bool HasAMother () {return particleDefinition !=NULL;}

  void Print() const;

private:
  G4int                 originalTrackID;
  G4ParticleDefinition* particleDefinition=NULL;
  G4Step                fLastStep;
  G4ThreeVector         originalPosition;
  G4ThreeVector         originalLocalPosition;
  G4ThreeVector         originalMomentum;
  G4ThreeVector         originalPolarization;
  G4double              originalEnergy;
  G4double              originalTime;

  G4VPhysicalVolume*    originalPreVolume = NULL;
  G4VPhysicalVolume*    originalPostVolume = NULL;

public:
  inline G4int GetOriginalTrackID() const {return originalTrackID;}
  inline G4ParticleDefinition* GetOriginalParticle() const {return particleDefinition;}
  inline G4ThreeVector GetOriginalPosition() const {return originalPosition;}
  inline G4ThreeVector GetOriginalLocalPosition() const {return originalLocalPosition;}
  inline G4ThreeVector GetOriginalMomentum() const {return originalMomentum;}
  inline G4ThreeVector GetOriginalPolarization() const {return originalPolarization;}
  inline G4double GetOriginalEnergy() const {return originalEnergy;}
  inline G4double GetOriginalTime() const {return originalTime;}
  inline G4VPhysicalVolume* GetOriginalPrePV() const {return originalPreVolume;}
  inline G4VPhysicalVolume* GetOriginalPostPV() const {return originalPostVolume;}

};

extern G4Allocator<KM3TrackInformation> aTrackInformationAllocator;

inline void* KM3TrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void KM3TrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator.FreeSingle((KM3TrackInformation*)aTrackInfo);}

#endif

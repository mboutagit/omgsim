#ifndef BDetectorConstruction_hh
#define BDetectorConstruction_hh

#include "G4UImessenger.hh"
#include "globals.hh"

#include "KM3DetectorConstruction.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalVolumeStore.hh"




using namespace std;
using namespace km3net;

namespace lactel
{
    
    class BDetectorConstruction : public KM3DetectorConstruction
    {
    private:
        enum class DetGeoConf{M2, M4, M6, M8, M10};
        //BDetectorConstruction();

    public:        
        using KM3DetectorConstruction::Construct;
        BDetectorConstruction();
        ~BDetectorConstruction();
        
        G4ThreeVector targetPosition;
        void setTargetPosition(G4ThreeVector);
        void setOMPosition(G4ThreeVector);

        G4VPhysicalVolume* Construct() override;
        void setTargetType(G4String);
        void setAcceptanceAngle(float angle);
      //  virtual G4LogicalVolume* Construct(const VolumeParameters* = NULL) override;

      void autoWorldSize();


        static BDetectorConstruction* GetIt();
        // static std::string className;

        G4double detSize;
        G4ThreeVector my_target_pos;
        G4double target_half_length;
        G4ThreeVector OMPosition; // w.r.t the INNER_LACTEL_VOLUME

        static float worldSizeOffset;
        static float acceptanceAngle; // angle used to compute the generation surface

        DetGeoConf detGeoConf;
    };
} // namespace lactel

#endif
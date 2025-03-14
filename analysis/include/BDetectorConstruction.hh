#ifndef BDetectorConstruction_hh
#define BDetectorConstruction_hh

#include "G4UImessenger.hh"
#include "globals.hh"

#include "KM3DetectorConstruction.hh"

using namespace std;


namespace km3net
{
    
    class BDetectorConstruction : public KM3DetectorConstruction
    {
    private:
        enum class DetGeoConf{M2, M4, M6, M8, M10};

    public:        
        using KM3DetectorConstruction::Construct;
        BDetectorConstruction();
        ~BDetectorConstruction();
        
        G4ThreeVector targetPosition;
        void setTargetPosition(G4ThreeVector);

        G4VPhysicalVolume* Construct() override;
        void setTargetType(G4String);
      //  virtual G4LogicalVolume* Construct(const VolumeParameters* = NULL) override;


        static BDetectorConstruction* GetIt();
        static std::string className;

        G4double detSize;
        G4ThreeVector my_target_pos;
        G4double target_half_length;

        DetGeoConf detGeoConf;
    };
} // namespace km3net

#endif
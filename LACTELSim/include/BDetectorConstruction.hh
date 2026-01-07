#ifndef BDetectorConstruction_hh
#define BDetectorConstruction_hh

#include "G4UImessenger.hh"
#include "globals.hh"

#include "KM3DetectorConstruction.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "BLogger.hh"




using namespace std;
using namespace km3net;
using namespace utils;

namespace lactel
{
    
    class BDetectorConstruction : public KM3DetectorConstruction
    {
    private:
        enum class DetGeoConf{M2, M4, M6, M8, M10};
        enum class DetectorState{ON, OFF, IDLE, RUN};
        //BDetectorConstruction();

    public:        
        using KM3DetectorConstruction::Construct;
        BDetectorConstruction();
        ~BDetectorConstruction();
        
        G4ThreeVector targetPosition;
        void setTargetPosition(G4ThreeVector);
        void setOMPosition(G4ThreeVector);
        double getOMPosition();

        G4VPhysicalVolume* Construct() override;
        void setTargetType(G4String);
        void setAcceptanceAngle(float angle);
        // virtual G4LogicalVolume* Construct(const VolumeParameters* = NULL) override;

        void DefineOpticalProperties();
        bool IsRunning(){if(state == DetectorState::RUN) return true; else return false;}
        bool useDAD(){return useDAQ;}
        void autoWorldSize();

        void ReadThisConfiguration (libconfig::Setting* cfg, // reads settings from KM3Det*.dat file
                                                       vector<PositionParameters>&  ConfList);

        G4VPhysicalVolume* PlaceThem (KM3ParametrizedVolume* theVolToPlace, PositionParameters& PosPar);

        static BDetectorConstruction* GetIt();
        static DetectorState state;
        static bool useDAQ;
        // static std::string className;

        G4double detSize;
        G4ThreeVector my_target_pos;
        G4double target_half_length;
        static double omPos;// w.r.t the INNER_LACTEL_VOLUME
        // G4ThreeVector omPos = G4ThreeVector(0, 0, -50 * cm); // w.r.t the INNER_LACTEL_VOLUME

        static float worldSizeOffset;
        static float acceptanceAngle; // angle used to compute the generation surface
        static G4ThreeVector OMPosition;
        float AGen = 0; // generation surface above the cell in cm²

        std::string detName;
        static std::string generationShape; // used to handle the command shape
        static G4ThreeVector genSurfPosition;
        static G4ThreeVector genSurfSize;

        

        void setGenerationShape(std::string shape);
        void setGenSurfShape(std::string shape);
        void setDetectorState(std::string state_){
            if(state_ == "ON")
                state = DetectorState::ON;
            else if(state_ == "OFF")
                state = DetectorState::OFF;
            else if(state_ == "IDLE")
                state = DetectorState::IDLE;
            else if(state_ == "RUN")
                state = DetectorState::RUN;
            else {
                BLogger::getInstance()->log(BLogger::Level::ERROR, "Unkown detector state !");  
                return;
            }
            BLogger::getInstance()->log(BLogger::Level::INFO, "Detector state: " + state_);
        }

        DetGeoConf detGeoConf;

        // class LactelWaterTank
        // {
        // protected:
        //     static int nObjetcs;
        //     int id;
        //     std::string name;
            
    
        // public:
        //     WaterTank(/* args */);
        //     ~WaterTank();
        // };
    };
} // namespace lactel

#endif
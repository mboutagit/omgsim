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

#include "OM.hh"

#include "G4ProductionCuts.hh"

#include "G4GDMLParser.hh"

#include "BLactelDetector.hh"
#include "BDetector.hh"


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
        BLactelDetector detector; // detector

    public:        
        using KM3DetectorConstruction::Construct;
        BDetectorConstruction();
        ~BDetectorConstruction();

        bool isADetectorState(DetectorState state){
            switch (state)
            {
            case DetectorState::ON:
            case DetectorState::OFF:
            case DetectorState::IDLE:
            case DetectorState::RUN:
                return true;
            
            default:
                return false;
            }
        }

        DetectorState strToDetectorState(std::string val){
            if(val == "ON" || val == "on")
                return DetectorState::ON;
            if(val == "OFF" || val == "off")
                return DetectorState::OFF;
            if(val == "IDLE" || val == "idle")
                return DetectorState::IDLE;
            if(val == "RUN" || val == "run")
                return DetectorState::RUN;
            
            throw runtime_error("Unkown Detector state " + val);
        }

        inline BLactelDetector& getDetector(){return detector;}
        
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
        bool useDAQStatus(){return useDAQ;}
        void autoWorldSize();
        
        void getOMIDbyVolumeName(std::string volumeName);
        void getOMsUsedbyVolumeName(std::string volumeName);
        void ReadConfiguration (string ConfigFile);

        void ReadThisConfiguration (libconfig::Setting* cfg, // reads settings from KM3Det*.dat file
                                                       vector<PositionParameters>&  ConfList);

        void ReadThisConfiguration (BLactelDetector& detector);
        void BuildDetectorVolumes (BLactelDetector& detector, std::string matName = "WaterVolumeInWaterTank");

        G4VPhysicalVolume* PlaceThem (KM3ParametrizedVolume* theVolToPlace, PositionParameters& PosPar);
        std::vector<G4VPhysicalVolume*> getPhysicalVolumesByName(std::string name);
        std::vector<G4VPhysicalVolume*> getPhysicalVolumesByNameLike(std::string name);

        bool isAnOM(G4VPhysicalVolume*);
        OM* getItsOM(G4VPhysicalVolume*);

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
        static bool autoGenSize;

        static std::set<std::string> fOMIDs;
        static std::map<OM*, G4VPhysicalVolume*> fOMPhysicalVolumes;
        
        void PrintAllMaterialProperties(ostream& out = std::cout);

    
        

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
                BLOG_INFO(Form("Unkown detector state %s! Exiting", state_.c_str()));  
                return;
            }
            BLOG_INFO(Form("Detector state: %s", state_.c_str()));
        }

        DetGeoConf detGeoConf;

        std::map<std::string, std::pair<G4VPhysicalVolume*, PositionParameters>> volumesMap;
        G4VPhysicalVolume* getPhysicalVolumeByName(std::string name);
        
        G4Region* lakeWaterRegion;
        G4ProductionCuts* lakeRegionCuts;


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
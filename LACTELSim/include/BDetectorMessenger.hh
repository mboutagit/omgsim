#ifndef BDetectorMessenger_hh
#define BDetectorMessenger_hh

#include "KM3DetectorMessenger.hh"

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"





class G4UIdirectory;
class BDetetcorConstruction;

using namespace km3net;

namespace lactel
{
    class BDetectorConstruction;

    class BDetectorMessenger : public KM3DetectorMessenger
    {
        private:
            G4UIdirectory* BDet;
            G4UIcmdWith3VectorAndUnit* targetPos_cmd;
            G4UIcmdWith3VectorAndUnit* OMPosition_cmd;
            G4UIcmdWith3VectorAndUnit* genSurfPosition_cmd;
            G4UIcmdWith3VectorAndUnit* genSurfSize_cmd;
            G4UIcmdWithADoubleAndUnit* targetLength_cmd;
            G4UIcmdWithADoubleAndUnit* acceptanceAngle_cmd;
            G4UIcmdWithAString* targetType_cmd;
            G4UIcmdWithAString* genSurfShape_cmd;
            G4UIcmdWithAString* detectorState_cmd;
            G4UIcmdWithABool* autoGenSize_cmd;

        
            BDetectorConstruction* BDetector;

        public:
            BDetectorMessenger(BDetectorConstruction* detConst);
            ~BDetectorMessenger();
            void SetNewValue(G4UIcommand*, G4String);

    };
} // namespace lactel


#endif
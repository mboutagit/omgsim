#ifndef BDetectorMessenger_hh
#define BDetectorMessenger_hh

#include "KM3DetectorMessenger.hh"
//#include "BDetectorConstruction.hh"

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"


class G4UIdirectory;
class BDetetcorConstruction;

namespace km3net
{
    class BDetectorConstruction;

    class BDetectorMessenger : public KM3DetectorMessenger
    {
        private:
            G4UIdirectory* BDet;
            G4UIcmdWith3VectorAndUnit* targetPos_cmd;
            G4UIcmdWithADoubleAndUnit* targetLength_cmd;
            G4UIcmdWithAString* targetType_cmd;

            BDetectorConstruction* BDetector;

        public:
            BDetectorMessenger(BDetectorConstruction* detConst);
            ~BDetectorMessenger();
            void SetNewValue(G4UIcommand*, G4String);

    };


    
} // namespace km3net


#endif
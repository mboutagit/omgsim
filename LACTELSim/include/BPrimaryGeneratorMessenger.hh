#ifndef BPrimaryGeneratorMessenger_hh
#define BPrimaryGeneratorMessenger_hh

#include "globals.hh"

#include "G4UImessenger.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"

#include "KM3RunMessenger.hh"

class G4UIdirectory;
class BDetetcorConstruction;

using namespace km3net;

namespace lactel
{
    class BPrimaryGeneratorAction;

    class BPrimaryGeneratorMessenger : public G4UImessenger
    {
        private:
            G4UIdirectory* LACTELDir;
            G4UIdirectory* BPGDir;
            G4UIcmdWithADoubleAndUnit* thetaGen_cmd;
            G4UIcmdWithADoubleAndUnit* EGen_cmd;
            G4UIcmdWithAString* genParsMode_cmd;
            G4UIcmdWithAnInteger* genType_cmd;

            G4UIcmdWithADoubleAndUnit* genEmin_cmd; // Emin and Emax in GeV
            G4UIcmdWithADoubleAndUnit* genEmax_cmd;

            G4UIcmdWithADoubleAndUnit* genThetaMin_cmd; // thetamax and thetamin in deg
            G4UIcmdWithADoubleAndUnit* genThetaMax_cmd;


            BPrimaryGeneratorAction* BPGA;

        public:
            BPrimaryGeneratorMessenger(BPrimaryGeneratorAction* bpga);
            ~BPrimaryGeneratorMessenger();
            void SetNewValue(G4UIcommand*, G4String);

    };
} // namespace lactel

#endif
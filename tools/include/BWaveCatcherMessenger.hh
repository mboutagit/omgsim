#ifndef BWaveCatcherMessenger_hh
#define BWaveCatcherMessenger_hh

#include "G4UImessenger.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "BWaveCatcher.hh"

namespace DAQ{
    class DAQManager;
}

using namespace DAQ;
namespace tools
{
    class BWaveCatcherMessenger : public G4UImessenger
    {
    private:
        /* data */

        G4UIdirectory* LACTELDir;
        G4UIdirectory* DAQDir;
        G4UIcmdWithABool* useDAQ_cmd;
        DAQManager* daqManager;
    public:
        BWaveCatcherMessenger(DAQManager* daqm);
        ~BWaveCatcherMessenger();
        void SetNewValue(G4UIcommand*, G4String);
    };
} // namespace tools

#endif
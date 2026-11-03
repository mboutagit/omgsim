#include "BWaveCatcherMessenger.hh"
#include "DAQManager.hh"

#include <iostream>


using namespace std;
using namespace km3net;
using namespace DAQ;
namespace tools
{
    BWaveCatcherMessenger::BWaveCatcherMessenger(DAQManager* daqm) : daqManager(daqm){
        LACTELDir = new G4UIdirectory("/LACTEL/");
        LACTELDir->SetGuidance("Commands related to lactel.");

        DAQDir = new G4UIdirectory("/LACTEL/DAQ/");
        DAQDir->SetGuidance("DAQManager settings");

        useDAQ_cmd = new G4UIcmdWithABool("/LACTEL/DAQ/useDAQ", this);
        useDAQ_cmd->SetGuidance("Command to set the DAQ ON or OFF");
        useDAQ_cmd->SetParameterName("useDAQ", false);
        useDAQ_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    };

    BWaveCatcherMessenger::~BWaveCatcherMessenger(){
        delete LACTELDir;
        delete DAQDir;
        delete useDAQ_cmd;
    };

    void BWaveCatcherMessenger::SetNewValue(G4UIcommand* cmd, G4String value){
        if(cmd == useDAQ_cmd){
            bool val = useDAQ_cmd->GetNewBoolValue(value);
            daqManager->setStatus(val);
        } 
    }



    
} // namespace tools

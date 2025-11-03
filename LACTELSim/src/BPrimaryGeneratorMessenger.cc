
#include "BPrimaryGeneratorMessenger.hh"
#include "BPrimaryGeneratorAction.hh"

#include <iostream>

#include "G4UImanager.hh"



using namespace std;
using namespace km3net;
namespace lactel
{
    BPrimaryGeneratorMessenger::BPrimaryGeneratorMessenger(BPrimaryGeneratorAction* bpga) : BPGA(bpga){
        LACTELDir = new G4UIdirectory("/LACTEL/");
        LACTELDir->SetGuidance("UI commands specific to this example.");

        BPGDir = new G4UIdirectory("/LACTEL/BPGA/");
        BPGDir->SetGuidance("BPrimaryGeneratorAction settings");

        thetaGen_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BPGA/theta", this);
        thetaGen_cmd->SetGuidance("Set generation zenith angle (for fixed zenith simulation)");
        thetaGen_cmd->SetParameterName("thetaGen", false);
        thetaGen_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        EGen_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BPGA/E", this);
        EGen_cmd->SetGuidance("Set generation Energy (for fixed Energy simulation)");
        EGen_cmd->SetParameterName("EGen", false);
        EGen_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        genParsMode_cmd = new G4UIcmdWithAString("/LACTEL/BPGA/genParsMode", this);
        genParsMode_cmd->SetGuidance("Set the BPGA generation parameters mode (fixed, integral, random and cos2)");
        genParsMode_cmd->SetParameterName("genPars", false);
        genParsMode_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        genType_cmd = new G4UIcmdWithAnInteger("/LACTEL/BPGA/genType", this);
        genType_cmd->SetGuidance("3 for for muons, 4 for dark box test. else, check KM3RunMessenger");
        genType_cmd->SetParameterName("choice", false);
        genType_cmd->AvailableForStates(G4State_PreInit);

        cout << "\r\033[20C" << "---->>>> Calling BPG Messenger" << endl;
    };

    BPrimaryGeneratorMessenger::~BPrimaryGeneratorMessenger(){
        delete LACTELDir;
        delete BPGDir;
        delete thetaGen_cmd;
        delete EGen_cmd;
        delete genParsMode_cmd;
        delete genType_cmd;
    };

    void BPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String value){
        if(cmd == thetaGen_cmd){
            BPGA->thetaGen = thetaGen_cmd->GetNewDoubleValue(value);
        } 
        else if(cmd == EGen_cmd){
            BPGA->EGen = EGen_cmd->GetNewDoubleValue(value);
        } 
        else if(cmd == genParsMode_cmd){
            BPGA->SetGenParsMode(value);
        } 
        else if(cmd == genType_cmd){
            int thetype=genType_cmd->GetNewIntValue(value);

            if(thetype == 3){ // for muons sim
                std::cout << "\r\033[10C" << "\033[1;35mInfo: BPrimaryGeneratorMessenger, muons mode.\033[0m" << std::endl;
                BPGA->initMuons ();
            }
            else if(thetype == 4){ // for dark box tests
                std::cout << "\r\033[10C" << "\033[1;35mInfo: BPrimaryGeneratorMessenger, test mode.\033[0m" << std::endl;
                BPGA->initTest (); 
            }
            else{ // for KM3 modes
                std::cout << "\r\033[10C" << "\033[1;35mInfo: BPrimaryGeneratorMessenger, mode not supported. Please switch to KM3RunMessenger modes. \nExiting ...\033[0m" << std::endl;
                exit(-1);
            }
        } 
    }



    
} // namespace lactel

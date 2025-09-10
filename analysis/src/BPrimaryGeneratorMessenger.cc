
#include "BPrimaryGeneratorMessenger.hh"
#include "BPrimaryGeneratorAction.hh"

#include <iostream>

#include "G4UImanager.hh"



using namespace std;

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

        cout << "\r\033[20C" << "---->>>> Calling BPG Messenger" << endl;
    };

    BPrimaryGeneratorMessenger::~BPrimaryGeneratorMessenger(){
        delete LACTELDir;
        delete BPGDir;
        delete thetaGen_cmd;
        delete EGen_cmd;
        delete genParsMode_cmd;
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
    }



    
} // namespace lactel

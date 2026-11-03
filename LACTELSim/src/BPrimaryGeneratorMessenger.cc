
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

        genEmin_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BPGA/Emin", this);
        genEmin_cmd->SetGuidance("Generator minimum energy with its unit");
        genEmin_cmd->SetParameterName("Emin", false);
        genEmin_cmd->AvailableForStates(G4State_PreInit);

        genEmax_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BPGA/Emax", this);
        genEmax_cmd->SetGuidance("Generator maximum energy with its unit");
        genEmax_cmd->SetParameterName("Emax", false);
        genEmax_cmd->AvailableForStates(G4State_PreInit);

        genThetaMin_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BPGA/thetaMin", this);
        genThetaMin_cmd->SetGuidance("Generator minimum zenith angle with its unit");
        genThetaMin_cmd->SetParameterName("thetaMin", false);
        genThetaMin_cmd->AvailableForStates(G4State_PreInit);

        genThetaMax_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BPGA/thetaMax", this);
        genThetaMax_cmd->SetGuidance("Generator maximum zenith angle with its unit");
        genThetaMax_cmd->SetParameterName("thetaMax", false);
        genThetaMax_cmd->AvailableForStates(G4State_PreInit);

        cout << "\r\033[20C" << "---->>>> Calling BPG Messenger" << endl;
    };

    BPrimaryGeneratorMessenger::~BPrimaryGeneratorMessenger(){
        delete LACTELDir;
        delete BPGDir;
        delete thetaGen_cmd;
        delete EGen_cmd;
        delete genParsMode_cmd;
        delete genType_cmd;
        delete genEmin_cmd;
        delete genEmax_cmd;
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
                BLOG_INFO("Muons mode.");
                BPGA->initMuons ();
            }
            else if(thetype == 4){ // for dark box tests
                BLOG_INFO("Test mode.");
                BPGA->initTest (); 
            }
            else{ // for KM3 modes
                BLOG_INFO("Mode not supported. Please switch to KM3RunMessenger modes");
                exit(-1);
            }
        } 

        else if(cmd == genEmin_cmd){
            BPGA->EMin = genEmin_cmd->GetNewDoubleValue(value);
        } 

        else if(cmd == genEmax_cmd){
            BPGA->EMax = genEmin_cmd->GetNewDoubleValue(value);
        }

        else if(cmd == genThetaMin_cmd){
            BPGA->thetaMin = genThetaMin_cmd->GetNewDoubleValue(value);
        } 

        else if(cmd == genThetaMax_cmd){
            BPGA->thetaMax = genThetaMax_cmd->GetNewDoubleValue(value);
        }

        else{
            throw runtime_error ("Unkown commande from BPGA");
        }
    }



    
} // namespace lactel

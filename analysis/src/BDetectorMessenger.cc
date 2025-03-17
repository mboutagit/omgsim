
#include "BDetectorMessenger.hh"
#include "BDetectorConstruction.hh"

#include <iostream>


using namespace std;

namespace km3net
{
  //  BDetectorMessenger::BDetectorMessenger(BDetectorConstruction* detConst) : KM3DetectorMessenger(detConst){};
    BDetectorMessenger::BDetectorMessenger(BDetectorConstruction* detConst) : KM3DetectorMessenger(detConst){
        BDet = new G4UIdirectory("/KM3/BDet/");
        BDet->SetGuidance("BDet detector type control.");
     
        targetPos_cmd = new G4UIcmdWith3VectorAndUnit("/KM3/BDet/targetPos", this);
        targetPos_cmd->SetGuidance("Set the position of the target.");
        targetPos_cmd->SetParameterName("x", "y", "z", false);
        targetPos_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        targetLength_cmd = new G4UIcmdWithADoubleAndUnit("/KM3/BDet/targetSize", this);
        targetLength_cmd->SetGuidance("Set the size of the target as box.");
        targetLength_cmd->SetParameterName("x", false);
        targetLength_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        targetType_cmd = new G4UIcmdWithAString("/KM3/BDet/targetType", this);
        targetType_cmd->SetGuidance("Set target volume type: M2 (2 m), M4, M6, M8, M10 .");
        targetType_cmd->SetParameterName("targetType", false);
        targetType_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);


        cout << "\r\033[20C" << "---- Calling det Messenger" << endl;


    };

    BDetectorMessenger::~BDetectorMessenger(){
        delete BDet;
        delete targetPos_cmd;
        delete targetLength_cmd;
        delete targetType_cmd;
    };

    void BDetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String value){
        KM3DetectorMessenger::SetNewValue(cmd, value);
        if(cmd == targetPos_cmd){
            myDetector->targetPos = targetPos_cmd->GetNew3VectorValue(value);
        } 
        else if(cmd == targetLength_cmd){
            myDetector->SetTargetLength(targetLength_cmd->GetNewDoubleValue(value));
        }
        else if(cmd == targetType_cmd){
            cout << "\r\033[20C" << "Target type set to: " << value << endl;
            BDetector->setTargetType(value);
        }
    }



    
} // namespace km3net

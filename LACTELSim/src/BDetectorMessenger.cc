
#include "BDetectorMessenger.hh"
#include "BDetectorConstruction.hh"

#include <iostream>

#include "G4UImanager.hh"



using namespace std;

namespace lactel
{
  //  BDetectorMessenger::BDetectorMessenger(BDetectorConstruction* detConst) : KM3DetectorMessenger(detConst){};
    BDetectorMessenger::BDetectorMessenger(BDetectorConstruction* detConst) : KM3DetectorMessenger(detConst){
        BDet = new G4UIdirectory("/LACTEL/BDet/");
        BDet->SetGuidance("BDet detector type control.");
     
        targetPos_cmd = new G4UIcmdWith3VectorAndUnit("/LACTEL/BDet/targetPos", this);
        targetPos_cmd->SetGuidance("Set the position of the target.");
        targetPos_cmd->SetParameterName("x", "y", "z", false);
        targetPos_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        targetLength_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BDet/targetSize", this);
        targetLength_cmd->SetGuidance("Set the size of the target as box.");
        targetLength_cmd->SetParameterName("x", false);
        targetLength_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        targetType_cmd = new G4UIcmdWithAString("/LACTEL/BDet/targetType", this);
        targetType_cmd->SetGuidance("Set target volume type: M2 (2 m), M4, M6, M8, M10 .");
        targetType_cmd->SetParameterName("targetType", false);
        targetType_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        genSurfShape_cmd = new G4UIcmdWithAString("/LACTEL/BDet/genSurfShape", this);
        genSurfShape_cmd->SetGuidance("Shape of the generation surface, Box, disk, sphere, hemisphere");
        genSurfShape_cmd->SetParameterName("shape", false);
        genSurfShape_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        genSurfPosition_cmd = new G4UIcmdWith3VectorAndUnit("/LACTEL/BDet/genSurfPosition", this);
        genSurfPosition_cmd->SetGuidance("Positon of the generation surface");
        genSurfPosition_cmd->SetParameterName("x", "y", "z", false);
        genSurfPosition_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        genSurfSize_cmd = new G4UIcmdWith3VectorAndUnit("/LACTEL/BDet/genSurfSize", this);
        genSurfSize_cmd->SetGuidance("Size of the generation surface");
        genSurfSize_cmd->SetParameterName("l_x", "l_y", "l_z", false);
        genSurfSize_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        OMPosition_cmd = new G4UIcmdWith3VectorAndUnit("/LACTEL/BDet/OMPosition", this);
        OMPosition_cmd->SetGuidance("Set the OM position w.r.t to the inner_lactel_cell volume");
        OMPosition_cmd->SetParameterName("OM_x", "OM_y", "OM_z", false);
        OMPosition_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

        acceptanceAngle_cmd = new G4UIcmdWithADoubleAndUnit("/LACTEL/BDet/acceptanceAngle", this);
        acceptanceAngle_cmd->SetGuidance("Set acceptance angle for events comming to the det.");
        acceptanceAngle_cmd->SetParameterName("Angle with unit", false);
        acceptanceAngle_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);


        cout << "\r\033[20C" << "---- Calling det Messenger" << endl;


    };

    BDetectorMessenger::~BDetectorMessenger(){
        delete BDet;
        delete targetPos_cmd;
        delete targetLength_cmd;
        delete targetType_cmd;
        delete OMPosition_cmd;
    };

    void BDetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String value){
        // KM3DetectorMessenger::SetNewValue(cmd, value);
        if(cmd == targetPos_cmd){
            BDetector->targetPos = targetPos_cmd->GetNew3VectorValue(value);
        } 
        else if(cmd == targetLength_cmd){
            BDetector->SetTargetLength(targetLength_cmd->GetNewDoubleValue(value));
        }
        else if(cmd == targetType_cmd){
            cout << "\r\033[20C" << "Target type set to: " << value << endl;
            BDetector->setTargetType(value);
        }
        else if(cmd == OMPosition_cmd){
            BDetector->OMPosition = OMPosition_cmd->GetNew3VectorValue(value);

            for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
                //place the OM 60 cm above the target volume
                if(pv->GetName() == "AntaresOM"){
                    pv->SetTranslation(BDetector->OMPosition);
                    std::cout << "\r\033[10C" << "PV: " << pv->GetName() << " Found! " << std::endl;
                    // G4UImanager::GetUIpointer()->ApplyCommand("/run/initialize");
                    G4UImanager::GetUIpointer()->ApplyCommand("/vis/drawVolume");
                }
            }
            std::cout << "\r\033[10C \033[1;33m" << "Set new OM Position: " << BDetector->OMPosition << "\033[0m" << std::endl;
        }
        else if (cmd == acceptanceAngle_cmd){
            BDetector->acceptanceAngle = targetLength_cmd->GetNewDoubleValue(value);
            std::cout << "\r\033[20C \033[1;33m" << "Set new acceptance angle (deg): " << BDetector->acceptanceAngle/deg << "\033[0m" << std::endl;

        }
        else if (cmd == genSurfShape_cmd){
            BDetector->setGenerationShape(value);
            std::cout << "\r\033[20C \033[1;33m" << "Generation surface Shape is: " << value << "\033[0m" << std::endl;
        }

        else if (cmd == genSurfPosition_cmd){
            BDetector->genSurfPosition = genSurfPosition_cmd->GetNew3VectorValue(value);
            std::cout << "\r\033[20C \033[1;33m" << "Generation surface position: " << BDetector->genSurfPosition  << "\033[0m" << std::endl;
        }

        else if (cmd == genSurfSize_cmd){
            BDetector->genSurfSize = genSurfSize_cmd->GetNew3VectorValue(value);
            std::cout << "\r\033[20C \033[1;33m" << "Generation surface size is: " << BDetector->genSurfSize << "\033[0m" << std::endl;
        }
    }



    
} // namespace lactel

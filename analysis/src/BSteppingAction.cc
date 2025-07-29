#include "BSteppingAction.hh"
#include "BAnalysis.hh"


namespace lactel
{
    
    // BSteppingAction* BSteppingAction::fInstance = new BSteppingAction();
    // BSteppingAction* BSteppingAction::GetIt(){
    //     if(!fInstance){
    //         return new BSteppingAction();
    //     }
    // }


    BSteppingAction::BSteppingAction(){
        std::cout << "\r\033[10C" << "Using BA Stepping Action" << std::endl;
    }

    BSteppingAction::~BSteppingAction(){

    }
    void BSteppingAction::UserSteppingAction(const G4Step* step){

        G4String volName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

        if (volName != "World") {
        // G4double edep = step->GetTotalEnergyDeposit();
            // G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
            // G4cout << "Pos at " << pos << G4endl;
            KM3SteppingAction::UserSteppingAction(step);

            KM3TrackInformation* info = (KM3TrackInformation*)(step->GetTrack()->GetUserInformation());
            if (info == 0) 
                return;
            
            G4VPhysicalVolume* thePreOriginalPV = info->GetOriginalPrePV ();
            G4VPhysicalVolume* thePostOriginalPV = info->GetOriginalPostPV ();
            
            if (!thePreOriginalPV || !thePostOriginalPV ){
                // cout << "A volume is messing !!" << endl;
                return;
            }
            
            G4String prestepName=thePreOriginalPV->GetLogicalVolume ()->GetName();
            G4String poststepName=thePostOriginalPV->GetLogicalVolume ()->GetName();
            G4Track* fTrack = step->GetTrack();

        
            if (KM3PMTOpticalModel::IsAPhotocathode (thePreOriginalPV, thePostOriginalPV) &&
                fTrack->GetDefinition()->GetParticleName() == "e-" &&
                info->GetOriginalParticleType () == "opticalphoton" &&
                step->GetPreStepPoint()->GetMaterial()->
                    GetMaterialPropertiesTable ()->ConstPropertyExists("electronSensitive"))

            {
                // KM3AnalysisManager::getIt()->AddPhotoTubeEntranceInfo (step);
                BAnalysis::getIt() -> AddPhotoTubeEntranceInfo(step);
            }
            else { 
                return;
            }
        }
        
    }

    bool BSteppingAction::tmpBSteppingActionBuilder = km3net::KM3ExperimentTypeMessenger::GetIt()->SetASteppingAction ("BA", []() { return new BSteppingAction(); } );

} // namespace lactel

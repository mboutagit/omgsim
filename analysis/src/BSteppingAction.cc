#include "BSteppingAction.hh"
#include "BDetectorConstruction.hh"
#include "BPrimaryGeneratorAction.hh"
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

         G4Box* targetBox;

         for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
            if(pv->GetName() == "LACTEL_Cell_Outer"){
                targetBox = dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid());
                // std::cout << "\r\033[10C" << "Box found, Name: "  << pv->GetName() << std::endl;
                break; // return the first outer cell found (should be handled for multi boxes)
            }
        } 

        // std::cout << "\r\033[55C" << "Checking hit possibility" << std::endl;
        if(! BPrimaryGeneratorAction::getInstance()->couldHitCube(BPrimaryGeneratorAction::getInstance()->particle_position,
                                                                BPrimaryGeneratorAction::getInstance()->particle_direction, 
                                                                *targetBox
                                                                )
            ){
                // std::cout << "\r\033[55C" << "NOT Possible to hit: P="
                //                             << BPrimaryGeneratorAction::getInstance()->particle_position
                //                             << "--- D="
                //                             << BPrimaryGeneratorAction::getInstance()->particle_direction
                //                             << std::endl;
                G4Track* fTrack = step->GetTrack();
                fTrack->SetTrackStatus(fStopAndKill);
                // std::cout << "\r\033[C" << "Track killed" << std::endl;
            }
        
        G4StepPoint* prestep = step->GetPreStepPoint();
        G4StepPoint* poststep = step->GetPostStepPoint();

        G4VPhysicalVolume* thePrePV=step->GetPreStepPoint()->GetPhysicalVolume();
        G4VPhysicalVolume* thePostPV=step->GetPostStepPoint()->GetPhysicalVolume();

        G4String volName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

       // if (volName != "World") {
        // G4double edep = step->GetTotalEnergyDeposit();
            // G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
            // G4cout << "Pos at " << pos << G4endl;
          //  KM3SteppingAction::UserSteppingAction(step);

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

            if (!thePrePV || !thePostPV || !thePreOriginalPV || !thePostOriginalPV ){
            // cout << "A volume is messing !!" << endl;
            // if(thePrePV) cout << "\r\033[10C" << "prePV name: " << thePrePV->GetLogicalVolume ()->GetName();
            // if(thePostPV) cout << " - postPV name: " << thePostPV->GetLogicalVolume ()->GetName();
            // if(thePreOriginalPV) cout << " - preOPV name: " << thePreOriginalPV->GetLogicalVolume ()->GetName();
            // if(thePostOriginalPV) cout << " - postOPV name: " << thePostOriginalPV->GetLogicalVolume ()->GetName();
            // std:: cout<<endl;
            return;
            }

            if (KM3DetectorConstruction::fOMNames.find(poststepName) != KM3DetectorConstruction::fOMNames.end()) {
                std::cout << "\r\033[25C" << "One OM hit: " << poststepName << std::endl;
            }

            if( KM3DetectorConstruction::fOMNames.find(thePostPV->GetName()) != KM3DetectorConstruction::fOMNames.end() &&  
                fTrack->GetDefinition()->GetParticleName() == "opticalphoton")
            {
                // std::cout << "\r\033[55C" << "YEEEEEEEEEEES" << std::endl;
               BAnalysis::getIt()->AddOMEntranceInfo(step);
            }
            // std::cout << "\r\033[55C" << "YEEEEEEEEEEES" << std::endl;
            // for (auto& name : KM3DetectorConstruction::fOMNames)
            // {
            //     std::cout << "\r\033[11C" << "name is: " << name << "  --- post name: " << poststepName << std::endl;
            // }
            
        
            if (KM3PMTOpticalModel::IsAPhotocathode (thePreOriginalPV, thePostOriginalPV) &&
                fTrack->GetDefinition()->GetParticleName() == "e-" &&
                info->GetOriginalParticleType () == "opticalphoton" &&
                step->GetPreStepPoint()->GetMaterial()->
                    GetMaterialPropertiesTable ()->ConstPropertyExists("electronSensitive"))

            {
                BAnalysis::getIt()->AddPhotoTubeEntranceInfo (step);
             //   BAnalysis::getIt()->AddOMEntranceInfo(step);
            }
            else { 
                return;
            }

            G4LogicalSurface* Surface =
            G4LogicalBorderSurface::GetSurface(thePreOriginalPV, thePostOriginalPV);
            if (!Surface)
            Surface = G4LogicalBorderSurface::GetSurface(thePostPV, thePrePV);
            if (!Surface)
            Surface = G4LogicalSkinSurface::GetSurface(thePostPV->GetLogicalVolume ());
            if (!Surface)
            Surface = G4LogicalSkinSurface::GetSurface(thePrePV->GetLogicalVolume ());

            if (!Surface ||
                !LoadMaterialProps (dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty())
                                    ->GetMaterialPropertiesTable()) )
            return;

            fTrack->SetTrackStatus(fStopAndKill); // kill e-

            fEnergy = info->GetOriginalEnergy ();

            G4ThreeVector globalPoint = step->GetPreStepPoint()->GetPosition ();
            const G4VTouchable*   touch = step->GetPostStepPoint()->GetTouchable();
            unsigned char detectedGamma = CheckDetection (touch->GetHistory()->GetTopTransform().TransformPoint(globalPoint));
            if (detectedGamma != 0){
            // std::cout << "\r\033[44C" << "detected particle!!" << std::endl;
            BAnalysis::getIt() ->AddDetectedParticule (info, detectedGamma);
            //G4cout << " -------- Detected particle. From KM3SteppingAction::UserSteppingAction" << G4endl;
            }



       // }
        
    }

    bool BSteppingAction::tmpBSteppingActionBuilder = km3net::KM3ExperimentTypeMessenger::GetIt()->SetASteppingAction ("BA", []() { return new BSteppingAction(); } );

} // namespace lactel

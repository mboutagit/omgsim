#include "BSteppingAction.hh"
#include "BDetectorConstruction.hh"
#include "BPrimaryGeneratorAction.hh"
#include "BAnalysisManager.hh"

#include "KM3PhysicsList.hh"
#include <G4RunManager.hh>


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
        // std::cout << "\r\033[10C" << "start steeping: " << std::endl;

        if( ! BDetectorConstruction::GetIt()->IsRunning()){
            return;
        }



        G4VPhysicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        // if (vol->GetName()) {
            // Prevent scintillation/Cerenkov photons
            // std::cout << "\r\033[50C" << "volume name: " << vol->GetName() << std::endl;
            // auto particle = step->GetTrack()->GetDefinition();
            // if (particle->GetParticleName() == "opticalphoton") {
            //     // std::cout << "\r\033[50C" << "killing the particle. " << std::endl;
            //     step->GetTrack()->SetTrackStatus(fStopAndKill);
            // }
        // }
            

        G4Track* fTrack = step->GetTrack();
        if(fTrack->GetParentID() == 0){
            myCostumTrackColor(step);
        }
            myCostumTrackColor(step);

        
        // std::cout << "\r\033[10C" << "GunType: " << static_cast<int> (BPrimaryGeneratorAction::getInstance()->getGunType()) << std::endl;
        if(static_cast<int> (BPrimaryGeneratorAction::getInstance()->getGunType()) == 3){
            G4Box* targetBox;
            std::string volumeName="IdealWaterVolume1";
            for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
                if(pv->GetName().rfind(volumeName, 0) == 0){ // find the first outer volume and break;
                    targetBox = dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid());
                    break; // return the first outer cell found (should be handled for multi boxes)
                }
            } 
            if(! targetBox){
                    std::cout << "\r\033[10C" << "Fatal: BSteppingAction::UserSteppingAction, Volume with name " << volumeName << " Not found!! \nExiting .." << std::endl;
                    return; // Prevent processing
            }
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
                    
                    fTrack->SetTrackStatus(fStopAndKill);
                    // std::cout << "\r\033[C" << "Track killed" << std::endl;
                }
            // std::cout << "\r\033[20C" << "---- after couldHitCube test " << std::endl;
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
            
            const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();

            G4String prestepName=thePreOriginalPV->GetLogicalVolume ()->GetName();
            G4String poststepName=thePostOriginalPV->GetLogicalVolume ()->GetName();
            // G4Track* fTrack = step->GetTrack();

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
               BAnalysisManager::getIt()->AddOMEntranceInfo(step);
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
                BAnalysisManager::getIt()->AddPhotoTubeEntranceInfo (step);
             //   BAnalysisManager::getIt()->AddOMEntranceInfo(step);
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

            // G4Material* mat = G4Material::GetMaterial("YourMaterialName");
                G4MaterialPropertiesTable* mpt = dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty())->GetMaterialPropertiesTable();
                
                // std::vector<G4String> propertyNames = {
                //     "RINDEX",
                //     "ABSLENGTH",
                //     "EFFICIENCY"
                // };

                // for (const auto& name : propertyNames) {
                    // G4MaterialPropertyVector* vec = _efficiency_photocathode; //mpt->GetProperty(name);
                    // if (vec) {
                    //     // std::cout << "Property: " << name << endl;
                    //     for (size_t i = 0; i < vec->GetVectorLength(); ++i) {
                    //         double energy = vec->Energy(i);
                    //         double value = (*vec)[i];
                    //         std::cout << " ---- lambda = " << energy << " m = " << energy*1e9 << " nm ,  Value = " << value << endl;
                    //     }
                    //     std::cout << std::endl;
                    // }
                // }

            fTrack->SetTrackStatus(fStopAndKill); // kill e-

            fEnergy = info->GetOriginalEnergy ();

            G4ThreeVector globalPoint = step->GetPreStepPoint()->GetPosition ();
            const G4VTouchable*   touch = step->GetPostStepPoint()->GetTouchable();
            unsigned char detectedGamma = CheckDetection (touch->GetHistory()->GetTopTransform().TransformPoint(globalPoint));
            // unsigned char detectedGamma = CheckDetection (step, info);
            if (detectedGamma != 0 && detectedGamma > 0){
                // std::cout << "\r\033[44C" << "detected particle!!" << std::endl;
                for (int ihist = 0; ihist < touch->GetHistoryDepth (); ihist++) {
                    G4String volname = touch->GetVolume(ihist)->GetName();
                    if (KM3DetectorConstruction::fOMNames.find(volname) != KM3DetectorConstruction::fOMNames.end()) {
                        BAnalysisManager::getIt()->getHit().om_id  = touch->GetCopyNumber(ihist);
                        // std::cout << "\r\033[10C" << "volume name: " << volname << std::endl;
                        // BDetectorConstruction::GetIt()->getOMsUsedbyVolumeName(volname);
                    }
                }
                BAnalysisManager::getIt() ->AddDetectedParticule (info, detectedGamma);
                
                //G4cout << " -------- Detected particle. From KM3SteppingAction::UserSteppingAction" << G4endl;
            }

       // }
        
    }

    // unsigned char BSteppingAction::CheckDetection(const G4ThreeVector& localPoint, KM3TrackInformation* info)
    // {
    //     unsigned char detectedGamma = 4;
    //     if (KM3PMTOpticalModel::GetLuxuryLevel() == 3) {
    //             //double eta_E = 0.5; //probability that the electron goes in or out the photocathode
    //             //from the photonis, can be improved. The abs length is ~30nm, the electron is produced at about an half of the photocathode.
    //             double teta = localPoint.theta ();//acos(costeta);
    //             double eta_teta=_angular_efficiency->Value(teta/3*2.1);
    //             double eta_teta_err=_angular_efficiency_err->Value(teta/3*2.1);

    //             double lambda = 1240./(info->GetOriginalEnergy() / CLHEP::eV);
    //             double qe = _efficiency_photocathode->Value(lambda);

    //             // G4MaterialPropertyVector* rindex = mpt->GetProperty("RINDEX");

    //             std::cout << "\r\033[20C" << "CheckDetection - teta: " << teta 
    //                                         << " - eta_teta: " << eta_teta 
    //                                         << " - eta_teta_err: " << eta_teta_err 
    //                                         << " - lambda: " << lambda 
    //                                         << " - qe: " << qe 
    //                                         << std::endl;

    //             double rndm = G4UniformRand();
    //             if(rndm < eta_teta*(1.-eta_teta_err))
    //             {
    //                 detectedGamma = 3;
    //             }
    //             else if(rndm < eta_teta)
    //             {
    //                 detectedGamma = 2;
    //             }
    //             else if (rndm < eta_teta*(1+eta_teta_err))
    //             {
    //                 detectedGamma = 1;
    //             }

    //     } else {
    //         double rndm = G4UniformRand();
    //         //G4cout << "DEBUG: converted" << G4endl;
    //         if (rndm < 0.9) {
    //             detectedGamma = 2;
    //             //G4cout << "DEBUG: detected" << G4endl;
    //         }
    //     }

    //     //KM3AnalysisManager* AAam = KM3AnalysisManager::getIt();
    //     //G4cout << "fKeepAll " << AAam->fKeepAll << G4endl; 

    //     return detectedGamma;


    // }


//      unsigned char BSteppingAction::CheckDetection (const G4Step* STEP, KM3TrackInformation* info)
//   {
//     // std::cout << "\r\033[10C" << "---->>> CheckDetection: " << std::endl;
//     G4VPhysicalVolume* thePreOriginalPV = info->GetOriginalPrePV ();
//     G4VPhysicalVolume* thePostOriginalPV = info->GetOriginalPostPV ();
//     G4VPhysicalVolume* thePrePV=STEP->GetPreStepPoint()->GetPhysicalVolume();
//     G4VPhysicalVolume* thePostPV=STEP->GetPostStepPoint()->GetPhysicalVolume();

//     // G4LogicalSurface* Surface = G4LogicalBorderSurface::GetSurface(thePrePV, thePostPV);
    
//     // if (!Surface)
//     //     Surface = G4LogicalBorderSurface::GetSurface(thePostPV, thePrePV);
//     // if (!Surface)
//     //     Surface = G4LogicalSkinSurface::GetSurface(thePostPV->GetLogicalVolume ());
//     // if (!Surface)
//     //     Surface = G4LogicalSkinSurface::GetSurface(thePrePV->GetLogicalVolume ());
//     // std::cout << "\r\033[10C" << "---->>> CheckDetection: Surface. " << Surface << std::endl;

//     // if (!Surface ||
//     //     !LoadMaterialProps (dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty())
//     //                         ->GetMaterialPropertiesTable()) )
//     //     return 0;

//     G4LogicalSurface* Surface =
//         G4LogicalBorderSurface::GetSurface(thePreOriginalPV, thePostOriginalPV);
//         if (!Surface)
//             Surface = G4LogicalBorderSurface::GetSurface(thePostPV, thePrePV);
//         if (!Surface)
//             Surface = G4LogicalSkinSurface::GetSurface(thePostPV->GetLogicalVolume ());
//         if (!Surface)
//             Surface = G4LogicalSkinSurface::GetSurface(thePrePV->GetLogicalVolume ());

//         if (!Surface ||
//             !LoadMaterialProps (dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty())
//                                 ->GetMaterialPropertiesTable()) )
//         return 0;


//     unsigned char detectedGamma = CalculateDetection (STEP);
//     // std::cout << "\r\033[10C" << "---- Try to check detection: detectedGamma = " << (int)detectedGamma << std::endl;

//     return detectedGamma;
//   }


//   unsigned char BSteppingAction::CalculateDetection(const G4Step* STEP)
//   {
//     // std::cout << "Hey, I am using CalculateDetection in KM3PhotoTubeMgr " << std::endl;
//     KM3PMTOpticalModel* theModel = ((KM3PhysicsList*)(G4RunManager::GetRunManager()->GetUserPhysicsList ()))->GetOpticalModel ();

//     G4ThreeVector globalPoint = STEP->GetPreStepPoint()->GetPosition ();
//     G4ThreeVector localPoint=STEP->GetPostStepPoint()->GetTouchable()->GetHistory()->GetTopTransform().TransformPoint(globalPoint);
//     double Energy = STEP->GetPreStepPoint()->GetKineticEnergy();
//     unsigned char detectedGamma = 0;
//     double Efficiency=_efficiency_photocathode->Value (Energy);
//     double eta_E = Efficiency/theModel->CalculateNormalCoefficients ();//\/OpticalModuleMaker::GetCurrentAbsorption ();

//     double teta = localPoint.theta ();//acos(costeta);

//     double eta_teta=_angular_efficiency->Value(teta/3*2.1);
//     double eta_teta_err=_angular_efficiency_err->Value(teta/3*2.1);

//     // std::cout << "\r\033[10C" << "OK, until here I am OK !!" << std::endl;
//     double rndm = G4UniformRand();
//     if(rndm < eta_E)
//         {
//             rndm=G4UniformRand();
//             if(rndm < eta_teta*(1.-eta_teta_err))
//             {
//                 detectedGamma = 3;
//             }
//             else if(rndm < eta_teta)
//             {
//                 detectedGamma = 2;
//             }
//             else if (rndm < eta_teta*(1+eta_teta_err))
//             {
//                 detectedGamma = 1;
//             }
//             else
//             {
//                 detectedGamma = 4;
//             }

//         }

//     return detectedGamma;
//   }
    bool BSteppingAction::tmpBSteppingActionBuilder = km3net::KM3ExperimentTypeMessenger::GetIt()->SetASteppingAction ("BA", []() { return new BSteppingAction(); } );

} // namespace lactel

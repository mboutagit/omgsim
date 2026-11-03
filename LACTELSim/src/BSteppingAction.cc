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
        BLOG_INFO("Using BA Stepping Action");
    }

    BSteppingAction::~BSteppingAction(){

    }
    void BSteppingAction::UserSteppingAction(const G4Step* step){
        if( ! BDetectorConstruction::GetIt()->IsRunning()){
            return;
        }

        G4VPhysicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        auto particle = step->GetTrack()->GetDefinition();
 
        G4Track* fTrack = step->GetTrack();
        if(fTrack->GetParentID() == 0){
            myCostumTrackColor(step);
        }
            myCostumTrackColor(step);

        G4StepPoint* prestep = step->GetPreStepPoint();
        G4StepPoint* poststep = step->GetPostStepPoint();

        G4VPhysicalVolume* thePrePV=step->GetPreStepPoint()->GetPhysicalVolume();
        G4VPhysicalVolume* thePostPV=step->GetPostStepPoint()->GetPhysicalVolume();

        G4String volName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

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

        if (!thePrePV || !thePostPV || !thePreOriginalPV || !thePostOriginalPV ){
        return;
        }

        if (KM3DetectorConstruction::fOMNames.find(poststepName) != KM3DetectorConstruction::fOMNames.end()) {
            BLOG_INFO(Form("One OM hits: %s", poststepName.c_str()));
        }

        if( KM3DetectorConstruction::fOMNames.find(thePostPV->GetName()) != KM3DetectorConstruction::fOMNames.end() &&  
            fTrack->GetDefinition()->GetParticleName() == "opticalphoton")
        {
            BAnalysisManager::getIt()->AddOMEntranceInfo(step);
        }
        
        if (KM3PMTOpticalModel::IsAPhotocathode (thePreOriginalPV, thePostOriginalPV) &&
            fTrack->GetDefinition()->GetParticleName() == "e-" &&
            info->GetOriginalParticleType () == "opticalphoton" &&
            step->GetPreStepPoint()->GetMaterial()->
                GetMaterialPropertiesTable ()->ConstPropertyExists("electronSensitive"))

        {
            BAnalysisManager::getIt()->AddPhotoTubeEntranceInfo (step);
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

        G4MaterialPropertiesTable* mpt = dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty())->GetMaterialPropertiesTable();

        fTrack->SetTrackStatus(fStopAndKill); // kill e- detected

        fEnergy = info->GetOriginalEnergy ();

        G4ThreeVector globalPoint = step->GetPreStepPoint()->GetPosition ();
        const G4VTouchable*   touch = step->GetPostStepPoint()->GetTouchable();
        unsigned char detectedGamma = CheckDetection (touch->GetHistory()->GetTopTransform().TransformPoint(globalPoint));
        if (detectedGamma != 0 && detectedGamma > 0){
            for (int ihist = 0; ihist < touch->GetHistoryDepth (); ihist++) {
                G4String volname = touch->GetVolume(ihist)->GetName();
                if (KM3DetectorConstruction::fOMNames.find(volname) != KM3DetectorConstruction::fOMNames.end()
                    || BDetectorConstruction::fOMIDs.find(volname) != BDetectorConstruction::fOMIDs.end()) {

                    OM* om = BDetectorConstruction::GetIt()->getItsOM(touch->GetVolume(ihist));
                    BLactelDetector det = BDetectorConstruction::GetIt()->getDetector();
                    BAnalysisManager::getIt()->getHit().om_id  = det.getOMIDByRefID(om->getOMID());
                    BAnalysisManager::getIt()->getHit().module  = det.getOMECell(om->getOMID())->getModule();
                    BAnalysisManager::getIt()->getHit().layer  = det.getOMECell(om->getOMID())->getLayer();
                }
            }
            BAnalysisManager::getIt() ->AddDetectedParticule (info, detectedGamma);
            BAnalysisManager::getIt() ->AddOMEntranceInfo(step);
        }
    }
    
    bool BSteppingAction::tmpBSteppingActionBuilder = km3net::KM3ExperimentTypeMessenger::GetIt()->SetASteppingAction ("BA", []() { return new BSteppingAction(); } );

} // namespace lactel

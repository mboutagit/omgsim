//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include <TTree.h>
#include <TFile.h>

#include "KM3AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4VTouchable.hh"
#include <G4NavigationHistory.hh>
#include <G4TransportationManager.hh>
#include <KM3OpticalModuleMgr.hh>  //for PMT name
#include <G4RunManager.hh>
#include <G4Run.hh>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

namespace km3net
{
  string KM3AnalysisManager::fRootFileName = "default.root";
  bool   KM3AnalysisManager::fKeepAll = false;
  bool   KM3AnalysisManager::fFastAnalysisOnly = true;
  bool   KM3AnalysisManager::fEvtOutput = true;
  short  KM3AnalysisManager::fVerbose = 0;
  std::ofstream KM3AnalysisManager::evt_stream;

  KM3AnalysisManager* KM3AnalysisManager::fManager = 0;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  KM3AnalysisManager* KM3AnalysisManager::getIt()
  {
    if(!fManager) {
      throw ("uncorrect use: a daugther of KM3AnalysisManager should be initializated first !!");
    }
    return fManager;
  }


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  KM3AnalysisManager::KM3AnalysisManager()
  {

  }

  KM3AnalysisManager::~KM3AnalysisManager()
  {

  }

  void KM3AnalysisManager::Init (string RFNAME)
  {

    if (RFNAME.size ())
      fRootFileName=RFNAME;

    fRFile = new TFile (fRootFileName.data (), "RECREATE");

    fTRunInfos = new TTree("RunInfo","Infos about the simulation.");
    fTRunInfos->Branch("NBTrigg",&fNbTrigg,"NbTrigg/l");
    fTEvent = new TTree("Hit","Hits in a TTree");
    fTEvent->Branch("Vertex",&run_id,"run_id/i:evt_id:VertexX/f:VertexY:VertexZ:VertexT:VertexP:VertexDirX:VertexDirY:VertexDirZ:VertexEnergy:OMHitX:OMHitY:OMHitZ:OMHitT:OMHitP:PhotoX:PhotoY:PhotoZ:PhotoT:PhotoP:OMID/S:PMID");
    fTFastAnalysis = new TTree("FastAnalysis","Data used for very fast analysis.");
    fTFastAnalysis->Branch("PhotonReach",&fNbOM,"NbOM/l:NbPhoto");

    //ANTARES evt-like format output
    if (fEvtOutput) {
        evt_stream.open("output.evt");
        evt_stream << "start_run: 1" << endl;
        evt_stream << "simul: OMGsim " << GIT_VERSION << "-" << BUILD_DATE << " " << KM3AnalysisManager::currentDateTime() << endl;
    }
  }

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string KM3AnalysisManager::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%y%m%d %H%M", &tstruct);

    return buf;
}



  bool KM3AnalysisManager::HasBeenInTheOMYet ()
  {
    if (fOMHitX==0 &&
        fOMHitY==0 &&
        fOMHitZ==0)
      return false;

    return true;
  }

  void KM3AnalysisManager::AddPhotoTubeEntranceInfo (const G4Step* STEP)
  {
    if (fNbPhoto ) //% 1000 ==0)
      {
       // cout << "\r\033[20C" << fNbPhoto << " in pmt ";
       // cout.flush ();
      }
    fNbPhoto++;
    if (fRFile == NULL)
      return;
    if (fFastAnalysisOnly)
      return;

    /*
     * More flexible and robust way of PMT number search introduced (scroll through all volumes).
     * Get OMid here was added here.
     */
    const G4VTouchable*   touchpost = STEP->GetPostStepPoint ()->GetTouchable ();
    for (int ihist = 0; ihist < touchpost->GetHistoryDepth (); ihist++) {
        G4String volname = touchpost->GetVolume(ihist)->GetName();
        if (volname.find(KM3PhotoTubeMgr::fPMTContainerName) != std::string::npos) {
             fPMID=touchpost->GetCopyNumber(ihist);
        } else if (KM3DetectorConstruction::fOMNames.find(volname) != KM3DetectorConstruction::fOMNames.end()) {
             fOMId=touchpost->GetCopyNumber(ihist);
        }
    }

    const G4StepPoint* prestep = STEP->GetPreStepPoint();
    const G4ThreeVector& globalPoint = prestep->GetPosition();
    const G4VTouchable*   touch = prestep->GetTouchable();
    const G4ThreeVector& localpoint=touch->GetHistory ()->GetTopTransform().TransformPoint(globalPoint);
    const G4ThreeVector norm=prestep->GetPhysicalVolume()->GetLogicalVolume ()->GetSolid ()->SurfaceNormal( localpoint );

    fPhotoX=localpoint.x ();
    fPhotoY=localpoint.y ();
    fPhotoZ=localpoint.z ();
    fPhotoT=localpoint.theta ();
    fPhotoP=localpoint.phi ();

  }

  //Vladimir: This function is outdated and not used currently. OMid is added at AddPhotoTubeEntranceInfo.
  void KM3AnalysisManager::AddOMEntranceInfo (const G4Step* STEP)
  {


    const G4StepPoint* prestep = STEP->GetPreStepPoint();
    G4String prestepName=prestep->GetPhysicalVolume()->GetName();

    if (prestepName != "Target")
      {
        return;
      }

    const G4StepPoint* poststep = STEP->GetPostStepPoint();
    G4String poststepName=poststep->GetPhysicalVolume()->GetName();

    if (fNbOM % 1000 ==0)
      {
        cout << endl;
        cout << "\r\033[40C" << fNbOM << " in OM ";
        cout.flush ();
      }
    fNbOM++;
    if (fRFile == NULL)
      return;
    if (fFastAnalysisOnly)
      return;

    const G4ThreeVector& globalPoint = poststep->GetPosition();
    const G4VTouchable*   touch = poststep->GetTouchable();
    const G4ThreeVector& localpoint=touch->GetHistory ()->GetTopTransform().TransformPoint(globalPoint);

    /*
     * G4int copyNo = theTouchable->GetCopyNumber(); 
     * G4int motherCopyNo = theTouchable->GetCopyNumber(1); 
     * where Copy here stays for any duplicated instance of a physical volume, 
     * either if it is a G4PVPlacement (multiple placements of the same logical volume) 
     * or a G4PVReplica/G4PVParameterised. 
     * The method GetCopyNo()is meant to return only the serial number of placements not duplicated in the geometry tree. 
     *
     * So, it seems, GetCopyNumber should be used instead of GetCopyNo since we do different placements of the same logical volume.
     */
   // cout <<" replica: " << touch->GetVolume ()->GetCopyNo () << endl;
    fOMId = touch->GetVolume ()->GetCopyNo ();
    //fOMId = touch->GetCopyNumber (2);

    fOMHitX = globalPoint.x ();
    fOMHitY = globalPoint.y ();
    fOMHitZ = globalPoint.z ();
    fOMHitT = localpoint.theta ();
    fOMHitP = localpoint.phi ();
    
  }

  void KM3AnalysisManager::StartOfEvent (const G4Event* EV)
  {

    fHits = 0;
    if (fRFile == NULL)
      return;

    fNbTrigg++;

    //G4cout << "start of event: " << EV->GetEventID() << G4endl;

    if ((fNbTrigg % 1000) == 0){
      cout << endl;
      G4cout<< "start of event: " << EV->GetEventID() << " || trigg: " << fNbTrigg << G4endl;
    }

     // cout  <<"\r"<<fNbTrigg << " trigg" << flush;

    if (fFastAnalysisOnly)
      return;

    const G4ThreeVector& vertexVect= EV->GetPrimaryVertex ()->GetPosition ();
    const G4ThreeVector& vertexDir = EV->GetPrimaryVertex ()->GetPrimary ()->GetMomentumDirection ();
    
    // fGenEvtID  = EV->GetEventID();    
    // fGenEvtID  = 
    
    //cout << "Run: " << G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID() << endl;
    run_id = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
    evt_id = EV->GetEventID();
    fVertexX = vertexVect.x ();
    fVertexY = vertexVect.y ();
    fVertexZ = vertexVect.z ();
    fVertexT = vertexVect.theta ();
    fVertexP = vertexVect.phi ();

    fVertexDirX = vertexDir.x ();
    fVertexDirY = vertexDir.y ();
    fVertexDirZ = vertexDir.z ();

    fVertexEnergy = EV->GetPrimaryVertex ()->GetPrimary ()->GetKineticEnergy ();
  }

  void KM3AnalysisManager::SetEnergy (double E)
  {
    if (fVerbose > 0)
      {
        cout << "KM3AnalysisManager::SetEnergy: nothing to do with the Energy " << E << endl;
      }

  }

  void KM3AnalysisManager::EndOfEvent ()
  {
    fVertexX=0;
    fVertexY=0;
    fVertexZ=0;
    fVertexT=0;
    fVertexP=0;
    fOMHitX=0;
    fOMHitY=0;
    fOMHitZ=0;
    fOMHitT=0;
    fOMHitP=0;
    fPhotoX=0;
    fPhotoY=0;
    fPhotoZ=0;
    fPhotoT=0;
    fPhotoP=0;
    fOMId=-1;
    fPMID=0;

    if (fEvtOutput && fHits != 0) {
        evt_stream << "end_event:" << endl;
    }
    
  }

}

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

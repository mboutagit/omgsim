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

#include "G4ios.hh"

#include "KM3DetectorConstruction.hh"
#include "KM3SteppingAction.hh"
#include "KM3AnalysisManager.hh"
#include <KM3OpticalModuleMgr.hh>
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"

#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>

#include <G4TransportationManager.hh>

#include <iostream>
#include <fstream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace CLHEP;

namespace km3net
{

  KM3SteppingAction::KM3SteppingAction()
  {

  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  KM3SteppingAction::~KM3SteppingAction()
  { }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  void KM3SteppingAction::UserSteppingAction(const G4Step* fStep)
  {
    //#ifdef G4ANALYSIS_USE
    G4Track* fTrack = fStep->GetTrack();
    G4StepPoint* prestep = fStep->GetPreStepPoint();
    G4StepPoint* poststep = fStep->GetPostStepPoint();

    if (fTrack->GetGlobalTime ()>1e10)
      {
        fTrack->SetGlobalTime (fTrack->GetLocalTime ());
        prestep->SetGlobalTime(prestep->GetLocalTime());
        poststep->SetGlobalTime(poststep->GetLocalTime());
      }

    if ( fTrack->GetDefinition()->GetParticleType() == "opticalphoton" && fStep->GetPostStepPoint()->GetPhysicalVolume() != 0)
      {

        //G4double xlambda = h_Planck*c_light/energy;

        G4VPhysicalVolume* thePrePV=fStep->GetPreStepPoint()->GetPhysicalVolume();
        G4VPhysicalVolume* thePostPV=fStep->GetPostStepPoint()->GetPhysicalVolume();

        G4String prestepName=thePrePV->GetLogicalVolume ()->GetName();
        G4String poststepName=thePostPV->GetLogicalVolume ()->GetName();
        G4String OMGlassName=KM3OpticalModuleMgr::fBentoName;

        if (  poststepName.rfind (OMGlassName) != string::npos)
          {
            KM3AnalysisManager::getIt ()->AddOMEntranceInfo (fStep);
          }

        if (fTrack->GetTrackStatus() != fStopAndKill)
          {
            //TEST TO REMOVE !!!
            if (thePostPV->GetName ()=="NOREFLEXVaccumInPmt")
              {
                fTrack->SetTrackStatus(fStopAndKill);
                return;
              }

            if (fStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary &&
                poststepName.rfind (KM3OpticalModuleMgr::fBentoName) < poststepName.size ())
              {
                KM3AnalysisManager::getIt()->AddOMEntranceInfo (fStep);
              }
          }
        else
          {
            G4bool isOnBoundary =
              (fStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary);
            if (!isOnBoundary)
              return;

            G4LogicalSurface* Surface = G4LogicalBorderSurface::GetSurface(thePrePV, thePostPV);
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

            KM3AnalysisManager::getIt()->AddPhotoTubeEntranceInfo (fStep);
            fEnergy = fStep->GetPreStepPoint()->GetKineticEnergy();

            G4ThreeVector globalPoint = fStep->GetPreStepPoint()->GetPosition ();
            const G4VTouchable*   touch = fStep->GetPostStepPoint()->GetTouchable();
            unsigned char detectedGamma = CheckDetection (touch->GetHistory()->GetTopTransform().TransformPoint(globalPoint));
            if (detectedGamma != 0)
              KM3AnalysisManager::getIt()->AddDetectedParticule (fStep, detectedGamma);

          }

      }
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  bool KM3SteppingAction::LoadMaterialProps (G4MaterialPropertiesTable* aMaterialPropertiesTable)
  {


    if (!aMaterialPropertiesTable) {
      return false;
    }
    //angular efficiency is an added propertyfor some additional calibration in addition to the thickness.
    _angular_efficiency_err= aMaterialPropertiesTable->GetProperty("ANGULAR_EFFICIENCY_ERROR");
    if (_angular_efficiency_err == NULL)
      {
        return false;
      }

    _angular_efficiency= aMaterialPropertiesTable->GetProperty("ANGULAR_EFFICIENCY");
    if (_angular_efficiency == NULL)
      {
        return false;
      }

    _efficiency_photocathode= aMaterialPropertiesTable->GetProperty("EFFICIENCY");
    if (_efficiency_photocathode == NULL)
      {
        return false;
      }

    _thickness_photocathode= aMaterialPropertiesTable->GetProperty("THICKNESS");
    if (_thickness_photocathode == NULL)
      {
        return false;
      }

    G4MaterialPropertyVector* kindex_photocathode= aMaterialPropertiesTable->GetProperty("KINDEX");
    if (kindex_photocathode == NULL)
      {
        return false;
      }

    G4MaterialPropertyVector* rindex_photocathode= aMaterialPropertiesTable->GetProperty("RINDEX");
    if (rindex_photocathode == NULL)
      {
        return false;
      }

    if (aMaterialPropertiesTable->ConstPropertyExists ("ElectronAbsLength")) fElectronAbsLength = aMaterialPropertiesTable->GetConstProperty ("ElectronAbsLength")*nm;

    /*aMaterialPropertiesTable->DumpTable ();
      exit (1);
    */
    return true;

  }

  unsigned char KM3SteppingAction::CheckDetection(const G4ThreeVector& localPoint)
  {

    unsigned char detectedGamma = 4;
    //double eta_E = 0.5; //probability that the electron goes in or out the photocathode
    //from the photonis, can be improved. The abs length is ~30nm, the electron is produced at about an half of the photocathode.
    double thickness=  _thickness_photocathode->Value( localPoint.theta() );

    double eta_E = exp (-thickness/2/fElectronAbsLength);
    //meeting some troubles, the theory should be checkout
    eta_E=0.5;
    double teta = localPoint.theta ();//acos(costeta);

    double eta_teta=_angular_efficiency->Value(teta/3*2.1);

    double eta_teta_err=_angular_efficiency_err->Value(teta/3*2.1);

    //cout << eta_teta  << ' ' << eta_teta_err << ' ' << teta/3*2.1*180/M_PI<<' ' << eta_E << endl;

    double rndm = G4UniformRand();
    //if(rndm < eta_E)
    //  {
    //    rndm=G4UniformRand();
    if(rndm < eta_E*eta_teta*(1.-eta_teta_err))
      {
        detectedGamma = 3;
      }
    else if(rndm < eta_E*eta_teta)
      {
        detectedGamma = 2;
      }
    else if (rndm < eta_E*eta_teta*(1+eta_teta_err))
      {
        detectedGamma = 1;
      }
    //}

    return detectedGamma;
  }

}

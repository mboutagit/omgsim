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
#include <KM3TrackInformation.hh>
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"

#include <KM3PMTOpticalModel.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>

#include <G4TransportationManager.hh>

#include "KM3Material.hh"
#include <iostream>
#include <fstream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace CLHEP;

namespace km3net
{

  KM3SteppingAction::KM3SteppingAction()
  {
    //test.push_back(1);
    fParticleColour["gamma"]       = G4Colour(0., 1., 0., 0.4); // green
    fParticleColour["e-"]          = G4Colour(1., 0., 0., 0.4); // red
    fParticleColour["e+"]          = G4Colour(0., 0., 1., 0.4); // blue
    fParticleColour["mu-"]         = G4Colour(1., 1., 0., 0.8); // yellow
    fParticleColour["mu+"]         = G4Colour(1., 0., 1., 0.8);
    fParticleColour["proton"]      = G4Colour(0.5, 0.5, 0., 0.4);
    fParticleColour["neutron"]     = G4Colour(0.5, 0., 0.5, 0.4);

    fParticleColour["opticalphoton"]     = G4Colour(0.9,0.5,0.1, 0.05);

    fParticleColour["nu_e"]        = G4Colour(.0, 0., 0.3, 0.2);
    fParticleColour["anti_nu_e"]   = G4Colour(.2, 0., 0.3, 0.2);
    fParticleColour["nu_mu"]       = G4Colour(.4, 0., 0.3, 0.2);
    fParticleColour["anti_nu_mu"]  = G4Colour(.6, 0., 0.3, 0.2);
    fParticleColour["nu_tau"]      = G4Colour(.8, 0., 0.3, 0.2);
    fParticleColour["anti_nu_tau"] = G4Colour(.1, 0., 0.3, 0.2);

  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  KM3SteppingAction::~KM3SteppingAction()
  { }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


  void KM3SteppingAction::myCostumTrackColor(const G4Step* fStep){
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (!pVVisManager)
      return;

      G4ThreeVector preStepPoint = fStep->GetPreStepPoint()->GetPosition();
      G4ThreeVector postStepPoint = fStep->GetPostStepPoint()->GetPosition();
      G4double charge = fStep->GetTrack()->GetDynamicParticle()->GetCharge();
      G4String particleName = fStep->GetTrack()->GetParticleDefinition()->GetParticleName();

      G4Polyline polyline;
      G4Colour colour;

      if(fParticleColour.find(particleName) != fParticleColour.end())
        colour = fParticleColour[particleName];
      else{
        colour = G4Colour(0.5, 0., 0., 0.5);
        //G4cout << "Particle Name: " << particleName << G4endl;
      }
        

      G4VisAttributes attribs(colour);
      attribs.SetForceSolid(true);  // Force solid rendering mode
      attribs.SetVisibility(true); 
      // attribs.SetLineWidth(3); // set visual width in pixels

      polyline.SetVisAttributes(attribs);
      polyline.push_back(preStepPoint);
      polyline.push_back(postStepPoint);
  

      //----- Call a drawing method for G4Polyline
      pVVisManager -> Draw(polyline);
  }

  void KM3SteppingAction::UserSteppingAction(const G4Step* fStep)
  {
    myCostumTrackColor(fStep);
    //#ifdef G4ANALYSIS_USE
    G4Track* fTrack = fStep->GetTrack();
    G4StepPoint* prestep = fStep->GetPreStepPoint();
    G4StepPoint* poststep = fStep->GetPostStepPoint();

    G4VPhysicalVolume* thePrePV=fStep->GetPreStepPoint()->GetPhysicalVolume();
    G4VPhysicalVolume* thePostPV=fStep->GetPostStepPoint()->GetPhysicalVolume();


    // cout  << "\r\033[20C" << "Material, electronSensitive: " 
    //       << mat->GetMaterialPropertiesTable ()->ConstPropertyExists("electronSensitive")
    //       << endl;

  // cout << " --- volumes: "  << thePrePV->GetName() << " - " << thePrePV->GetLogicalVolume()->GetName() << " -|- " 
  //                           << thePostPV->GetName() << " - " << thePostPV->GetLogicalVolume()->GetName() 
  //                           << endl;
    //moved here temporally by Vladimir since it seems that for the photons "info" is not created
    if (thePostPV != NULL) {
      //if (KM3PMTOpticalModel::IsAPhotocathode (fStep))   //VLA: It never enters here!!!!
      {
        if (fTrack->GetDefinition()->GetParticleType() == "opticalphoton" &&
            fTrack->GetTrackStatus() != fStopAndKill)
          {
         //   G4cout << " ----------- Optical photon detected " << G4endl;
            //if (thePostPV->GetName ()=="NOREFLEXVaccumInPmt")
            string vac = thePostPV->GetName ();
           // cout << "\r\033[40C" << "Name -----> pre: " << thePrePV->GetName () << " -- post: " << thePostPV->GetName() << endl;
            if (vac.find("NOREFLEXVaccumInPmt")!=std::string::npos)
            //VaccumOfPmt  was NOREFLEXVaccumInPmt
              {
                //G4cout << "   ----------- Potential Detection " << G4endl;
                fTrack->SetTrackStatus(fStopAndKill);
                cout << "\r\033[10C" << "--- M3SteppingAction::UserSteppingAction: Stop and killed!!" << endl;
                return;
              }
          }
      }
    }



    KM3TrackInformation* info = (KM3TrackInformation*)(fStep->GetTrack()->GetUserInformation());
    if (info == 0) 
      return;

    G4VPhysicalVolume* thePreOriginalPV = info->GetOriginalPrePV ();
    G4VPhysicalVolume* thePostOriginalPV = info->GetOriginalPostPV ();


  // if(!thePrePV) cout << " The thePrePV not found" << endl;
  // else cout << " The thePrePV name: " << thePrePV->GetName() << endl;
  // if(!thePostPV) cout << " The thePostPV not found" << endl;
  // else cout << " The thePostPV name: " << thePostPV->GetName() << endl;
  // if(!thePreOriginalPV) cout << " The thePreOriginalPV not found" << endl;
  // if(!thePostOriginalPV) cout << " The thePostOriginalPV not found" << endl;

  // cout << " --- volumes: "  << thePrePV->GetName() << " - " << thePostPV->GetName() << " -|- " 
  //                           << thePreOriginalPV->GetName() << " - " << thePostOriginalPV->GetName() 
  //                           << endl;

    if (!thePrePV || !thePostPV || !thePreOriginalPV || !thePostOriginalPV ){
      // cout << "A volume is messing !!" << endl;
      // if(thePrePV) cout << "\r\033[10C" << "prePV name: " << thePrePV->GetLogicalVolume ()->GetName();
      // if(thePostPV) cout << " - postPV name: " << thePostPV->GetLogicalVolume ()->GetName();
      // if(thePreOriginalPV) cout << " - preOPV name: " << thePreOriginalPV->GetLogicalVolume ()->GetName();
      // if(thePostOriginalPV) cout << " - postOPV name: " << thePostOriginalPV->GetLogicalVolume ()->GetName();
      // std:: cout<<endl;
       return;
    }

    // cout << "\r\033[10C" << "prePV name: " << thePrePV->GetLogicalVolume ()->GetName() 
    // << " - postPV name: " << thePostPV->GetLogicalVolume ()->GetName()
    // << " - preOPV name: " << thePreOriginalPV->GetLogicalVolume ()->GetName()
    // << " - postOPV name: " << thePostOriginalPV->GetLogicalVolume ()->GetName()
    // << std::endl;
     
    // else
    
    // cout << " --- volumes: "  << thePrePV->GetName()          << " - " << thePrePV->GetLogicalVolume()->GetName()          << " -|- " 
    //                           << thePostPV->GetName()         << " - " << thePostPV->GetLogicalVolume()->GetName()         << " -|- "
    //                           << thePreOriginalPV->GetName()  << " - " << thePreOriginalPV->GetLogicalVolume()->GetName()  << " -|- "
    //                           << thePostOriginalPV->GetName() << " - " << thePostOriginalPV->GetLogicalVolume()->GetName() 
    //                           << endl;

    G4String prestepName=thePreOriginalPV->GetLogicalVolume ()->GetName();
    G4String poststepName=thePostOriginalPV->GetLogicalVolume ()->GetName();

    //KM3AnalysisManager::getIt()->AddOMEntranceInfo (fStep);
      

  // if(fTrack->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
  //    //cout << "\r\033[10C" << " --- opticalphoton energy in eV: "  << fTrack->GetTotalEnergy() / eV << endl; 
     
  // }
  // cout << "\r\033[10C" << " --- After: "  << fTrack->GetParticleDefinition()->GetParticleName() << " -- "
  //                                         << "fTrack->GetUserInformation()->Print()"
  //                                         << endl;
    if(false)
    //if(info->GetOriginalParticleType () == "opticalphoton")
    //if(fTrack->GetDefinition()->GetParticleName() == "opticalphoton"
    //    && KM3PMTOpticalModel::IsAPhotocathode (thePreOriginalPV, thePostOriginalPV))
    cout  << "Cond: IsPho: "    << KM3PMTOpticalModel::IsAPhotocathode (thePreOriginalPV, thePostOriginalPV)
          << " / preSN: "       << prestepName 
          << " / PosS: "        << poststepName 
          << " / Particle: "    << fTrack->GetDefinition()->GetParticleName() 
          << " / Original: "    << info->GetOriginalParticleType ()
          << " / Prop: " << fStep->GetPreStepPoint()->GetMaterial()->GetMaterialPropertiesTable ()->ConstPropertyExists("electronSensitive")
          << endl;

    if (KM3PMTOpticalModel::IsAPhotocathode (thePreOriginalPV, thePostOriginalPV) &&
        fTrack->GetDefinition()->GetParticleName() == "e-" &&
        info->GetOriginalParticleType () == "opticalphoton" &&
        //prestepName.find (KM3PhotoTubeMgr::fPhotoName) != string::npos) 
        /*
         * VK: this does not work for the photons going back from the PMT vacuum (if first they were transmitted inside PMT
         * So, this should be used:
         * prestepName.find (KM3PhotoTubeMgr::fPhotoName) != string::npos) || postepName.find (KM3PhotoTubeMgr::fPhotoName) != string::npos)  
         * We decided not to do this check anyway (since it is enough that parent photon was in the photocathode.
         * Instead, we add below the check that the electron should be in the PMT sensitive area (PMT vacuum).
         */
        fStep->GetPreStepPoint()->GetMaterial()->
                        GetMaterialPropertiesTable ()->ConstPropertyExists("electronSensitive"))

      {
        KM3AnalysisManager::getIt()->AddPhotoTubeEntranceInfo (fStep);
        //G4cout << "  ------ PhototubeEntrance hit. From KM3SteppingAction::UserSteppingAction" << G4endl;
      }
    else {  //do nothing
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

    G4ThreeVector globalPoint = fStep->GetPreStepPoint()->GetPosition ();
    const G4VTouchable*   touch = fStep->GetPostStepPoint()->GetTouchable();
    unsigned char detectedGamma = CheckDetection (touch->GetHistory()->GetTopTransform().TransformPoint(globalPoint));
    if (detectedGamma != 0){
      KM3AnalysisManager::getIt()->AddDetectedParticule (info, detectedGamma);
      // G4ThreeVector creationPoint = fTrack->GetVertexPosition();

      //G4cout << " -------- Detected particle. From KM3SteppingAction::UserSteppingAction" << G4endl;
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

  /*
   * Adding angular efficiency here (this efficiency is the missing collection efficiency).
   * Note that angle is related to the PMT while thereis an internal rough correction to angle on the OM teta/3*2.1 which was valid only for ANTARES but kept there for consistency.
   * Output:
   * 4 - not detected
   * 3 - detected with angular efficiency - angular efficiency error
   * 2 - detected with angular efficiency but not with (angular efficiency - angular efficiency error)
   * 1 - detected with angular efficiency + angular efficiency error but not with angular efficiency
   */
  unsigned char KM3SteppingAction::CheckDetection(const G4ThreeVector& localPoint)
  {
	  unsigned char detectedGamma = 4;
	  if (KM3PMTOpticalModel::GetLuxuryLevel() == 3) {
		    //double eta_E = 0.5; //probability that the electron goes in or out the photocathode
		    //from the photonis, can be improved. The abs length is ~30nm, the electron is produced at about an half of the photocathode.
		    double teta = localPoint.theta ();//acos(costeta);

		    double eta_teta=_angular_efficiency->Value(teta/3*2.1);
        
		    double eta_teta_err=_angular_efficiency_err->Value(teta/3*2.1);

		    double rndm = G4UniformRand();
		    if(rndm < eta_teta*(1.-eta_teta_err))
		      {
			      detectedGamma = 3;
		      }
		    else if(rndm < eta_teta)
		      {
			      detectedGamma = 2;
		      }
		    else if (rndm < eta_teta*(1+eta_teta_err))
		      {
			      detectedGamma = 1;
		      }

	  } else {
		  double rndm = G4UniformRand();
		  //G4cout << "DEBUG: converted" << G4endl;
		  if (rndm < 0.9) {
			  detectedGamma = 2;
			  //G4cout << "DEBUG: detected" << G4endl;
		  }
	  }

    //KM3AnalysisManager* AAam = KM3AnalysisManager::getIt();
    //G4cout << "fKeepAll " << AAam->fKeepAll << G4endl; 

	  return detectedGamma;


  }

}

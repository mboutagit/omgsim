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

#include "MottaSteppingAction.hh"
#include "KM3AnalysisManager.hh"
#include "G4Track.hh"
#include "globals.hh"
#include <KM3ExperimentTypeMessenger.hh>

#include <iostream>
#include <fstream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace CLHEP;

namespace km3net
{
  bool MottaSteppingAction::tmpMottaSteppingActionBuilder =km3net::KM3ExperimentTypeMessenger::GetIt()->SetASteppingAction ("Motta", (km3net::MottaSteppingAction::GetIt));
  MottaSteppingAction* MottaSteppingAction::fInstance = 0;

  MottaSteppingAction* MottaSteppingAction::GetIt ()
  {
    if (!fInstance)
      fInstance = new MottaSteppingAction;
    return fInstance;
  }

  MottaSteppingAction::MottaSteppingAction()
  {

  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  MottaSteppingAction::~MottaSteppingAction()
  { }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

  void MottaSteppingAction::UserSteppingAction(const G4Step* fStep)
  {
    //#ifdef G4ANALYSIS_USE
    G4Track* fTrack = fStep->GetTrack();
    G4StepPoint* poststep = fStep->GetPostStepPoint();

    G4VPhysicalVolume* thePostPV=poststep->GetPhysicalVolume();
    G4String poststepName=thePostPV->GetLogicalVolume ()->GetName();

    //TODO check if it is relevent
    if (fTrack->GetDefinition()->GetParticleType() == "opticalphoton")
      {
        if (poststepName == "Target")
          fTrack->SetTrackStatus(fStopAndKill);
      }

    return KM3SteppingAction::UserSteppingAction (fStep);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
}

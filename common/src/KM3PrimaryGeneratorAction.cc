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

// **********************************************************************

#include <G4Neutron.hh>

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include <G4Navigator.hh>
#include <G4VPhysicalVolume.hh>
#include <G4TransportationManager.hh>

#include "KM3PrimaryGeneratorAction.hh"
#include <KM3DetectorConstruction.hh>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;
using namespace CLHEP;
namespace km3net
{

  KM3PrimaryGeneratorAction::KM3PrimaryGeneratorAction()
  {
    particleGun = 0;//new G4GeneralParticleSource();
  }

  KM3PrimaryGeneratorAction::~KM3PrimaryGeneratorAction()
  {
    if (particleGun != NULL)
      delete particleGun;
  }

  void KM3PrimaryGeneratorAction::InitializeNormalType ()
  {
    if (fGunType)
      throw ("Two guntype init in the mac file !!");

    cout << "Initializing normal type" << endl;

    particleGun = new G4GeneralParticleSource ();
    fGunType = 1;
    cout << "from here!! " << endl; // Bouta
  }

  void KM3PrimaryGeneratorAction::InitializeDecayType ()
  {
    if (fGunType)
      throw ("Two guntype init in the mac file !!");

    fTargetRadius=KM3DetectorConstruction::GetIt()->GetTargetFullLength();

    particleGun = new G4ParticleGun ();
    randcount = 0;
    Navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    fGunType = 2;
  }

/**
 * Custom fucntion for using muons for GPS. It should 
 * contain the generation geometry and random generation 
 * memuntum and energy based on PDG
 */

  void KM3PrimaryGeneratorAction::initMuons ()
    {
      if (fGunType)
        throw ("Two guntype init in the mac file !!");

      particleGun = new G4ParticleGun();
      fGunType = 3;
    }

  void KM3PrimaryGeneratorAction::InitializeCustumMuonType()
  {
   // cout << "\r\033[30C" << "From function -- Using custom muons generation!" << endl;
    // if (fGunType)
    //   throw ("Two guntype init in the mac file !!");
    // Create the particle gun
    KM3DetectorConstruction* det = KM3DetectorConstruction::GetIt();

    // G4VPhysicalVolume* detPhysic;
    // if(!detPhysic){
    //     G4VPhysicalVolume* (KM3DetectorConstruction::*physConstruct)() = &KM3DetectorConstruction::Construct;
    //     detPhysic = (det->*physConstruct)();
    // }
      
    // G4VPhysicalVolume* detPhysic = det->Construct();
     G4Sphere* genVolume = dynamic_cast<G4Sphere*> (det->genSolid);
    if(!genVolume){
      throw("KM3PrimaryGeneratorAction::InitializeCustumMuonType: Generation volume is not defined");
    }
  
    fTargetRadius = det->GetTargetFullLength();
    fGenVolumeRaduis = det->GetWorldFullLength();
    Navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    fGunType = 3;

    particleGun = new G4ParticleGun(); 

   // G4Sphere* sphere = dynamic_cast<G4Sphere*> (det->genSolid);  
    G4ThreeVector surfPoints = genVolume->GetPointOnSurface();

    while(surfPoints.getTheta() > CLHEP::pi/8) // only event with 60 deg at most
      surfPoints = genVolume->GetPointOnSurface();

    // G4ThreeVector surfPointNormals = surfPoints.unit();
    // surfPointNormals.setZ(- surfPointNormals.getZ());

    //cout << "\r\033[25C" << "----- GPS: thetta " << surfPoints.getTheta() << endl;

    // Set default particle type to muon-
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    G4String particleName;
    if(G4UniformRand() > 0.5)
      particleName = "mu+";
    else
      particleName = "mu-";

    //particleName = "mu-";
   // surfPoints = G4ThreeVector(0., 0., -300.);
    G4double energy = GenerateMuonEnergy(2 * GeV, 6 * GeV, 2.7);
    G4ThreeVector direction = GenerateMuonDirection("vertical");
    if(direction.getZ() > 0)
        direction.setZ(- direction.getZ());
    G4ThreeVector polarization(G4UniformRand(), G4UniformRand(), 0.0); 
    ((G4ParticleGun*)particleGun)->SetParticlePolarization(polarization);



    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    if(!particle)
      return;

    ((G4ParticleGun*)particleGun)->SetParticleDefinition(particle);
    ((G4ParticleGun*)particleGun)->SetParticlePosition(surfPoints);

     ((G4ParticleGun*)particleGun)->SetParticleEnergy(energy);
     ((G4ParticleGun*)particleGun)->SetParticleMomentumDirection(direction); // Toward the ground

    // Generate the particle
   // particleGun->GeneratePrimaryVertex(anEvent);

    //  cout << "\r\033[20C" << "GPS: generated particle name: " << particleName << endl; 
    // cout << "\r\033[20C" << "GPS: generated particle energy: " << energy << endl; 
    // cout << "\r\033[20C" << "GPS: generated particle Direction: " << direction << endl; 
    //  cout << "\r\033[20C" << "GPS: generated particle position: " << surfPoints << endl; 
    //  cout << "\r\033[20C" << "GPS: generated particle momentum: " << surfPointNormals << endl; 


    KM3AnalysisManager* amgr = KM3AnalysisManager::getIt();

    if(!amgr)
      return;

    amgr->setGenEnergy(energy);
    amgr->setPPDGID(particle->GetPDGEncoding());

    amgr->setGenPosition(surfPoints);
    amgr->setAngles(direction);
    amgr->setGenDirection(direction);

    //cout << "\r\033[25C" << "Filling info: " << surfPoints.getTheta() << endl;
    amgr->getGenEventTree()->Fill();
  }
  

  void KM3PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
  {
    string toprint;
    if (fGunType == 1)
      {
        particleGun->GeneratePrimaryVertex(anEvent);
        //cout << "Event generated at: " << anEvent->GetPrimaryVertex()->GetPosition().mag() << endl;
        return;
      }

    if (fGunType == 2)
      {
        G4VPhysicalVolume* initialVolume = NULL;

        G4ThreeVector InitPos;

        do
          {
            InitPos.setRThetaPhi(fTargetRadius * pow(G4UniformRand(), 1./3),
                                 acos(2*G4UniformRand()-1),
                                 twopi*G4UniformRand());

            initialVolume = Navigator->LocateGlobalPointAndSetup(InitPos);

            initialVolume?toprint=initialVolume->GetName ():toprint="NULL!";
          }
        while (initialVolume == NULL || initialVolume->GetName () != "Target");



        particleGun->SetParticlePosition (InitPos);


        ((G4GeneralParticleSource*)particleGun)->GeneratePrimaryVertex(anEvent);


        return;

      }

      if(fGunType == 3){
        //cout << "\r\033[20C" << "GeneratePrimaryVertix: Event ID = " << anEvent->GetEventID() << endl; 
        InitializeCustumMuonType();
        ((G4ParticleGun*)particleGun)->GeneratePrimaryVertex(anEvent);
        return;
      }

    string tosend("Gun type not initialized in mac file: fGunType");
    tosend+=(char)(fGunType+91);

    throw (tosend.data ());

  }

 G4ThreeVector KM3PrimaryGeneratorAction::GenerateMuonDirection(std::string mode) {
  G4double theta = std::acos(std::pow(G4UniformRand(), 1.));  // cos²(θ) distribution
  G4double  phi = 2 * CLHEP::pi * G4UniformRand(); // Uniform in azimuth
  
  if(mode == "vertical"){
    theta = CLHEP::pi;  // vertiacal -z
    phi = 2 * CLHEP::pi * G4UniformRand(); // Uniform in azimuth 
  }

  G4double ux = std::sin(theta) * std::cos(phi);
  G4double uy = std::sin(theta) * std::sin(phi);
  G4double uz = std::cos(theta);

  G4ThreeVector vect = G4ThreeVector(ux, uy, uz);
  return vect;
}

G4double KM3PrimaryGeneratorAction::GenerateMuonEnergy(double min, double max, float _alpha = 2.7) {
    return std::pow((std::pow(max, 1-_alpha) - std::pow(min, 1-_alpha)) 
           * G4UniformRand() 
           + std::pow(min, 1-_alpha), 1/(1-_alpha));
}

}

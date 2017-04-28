/** @file GLG4PMTOpticalModel.hh
    Defines a FastSimulationModel class for handling optical photon
    interactions with PMT: partial reflection, transmission, absorption,
    and hit generation.

    This file is part of the GenericLAND software library.
    $Id: GLG4PMTOpticalModel.hh,v 1.3 2005/03/29 19:58:06 GLG4sim Exp $

    @author Glenn Horton-Smith, March 20, 2001.
    @author Dario Motta, Feb. 23 2005: Formalism light interaction with photocathode.
*/

#ifndef __KM3PMTOpticalModel_hh__
#define __KM3PMTOpticalModel_hh__

#include "G4VFastSimulationModel.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UImessenger.hh"
#include <G4OpBoundaryProcess.hh>
#include <G4SystemOfUnits.hh>

class G4UIcommand;
class G4UIdirectory;

class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;

class TF1;

namespace km3net
{

  using namespace CLHEP;

  class KM3PMTOpticalModel : public G4UImessenger,
                             public G4OpBoundaryProcess
  {
    friend class KM3PMTOpticalModelTester;

  public:
    //-------------------------
    // Constructor, destructor
    //-------------------------
    KM3PMTOpticalModel ();

    // Note: There is no GLG4PMTOpticalModel(G4String) constructor.
    ~KM3PMTOpticalModel ();

    //------------------------------
    // Virtual methods of the base
    // class to be coded by the user
    //------------------------------

    void SetNewValue (G4UIcommand* command,G4String newValue);

    double CalculateNormalCoefficients(); // calculate and set fR_n, etc. and return the normal absorption
    // following two methods are for G4UImessenger
    //void SetNewValue(G4UIcommand * command,G4String newValues);
    //G4String GetCurrentValue(G4UIcommand * command);

  private:
    G4VParticleChange* PostStepDoIt (const G4Track &aTrack, const G4Step &aStep);



    //new boundary method elements:
    G4MaterialPropertyVector * _rindexPre = NULL;        // function of photon energy
    G4MaterialPropertyVector * _rindexPost = NULL;       // function of photon energy





    // material property vector pointers, initialized in constructor,
    // so we don't have to look them up every time DoIt is called.
    G4MaterialPropertyVector * _rindex_photocathode; // function of photon energy
    G4MaterialPropertyVector * _kindex_photocathode; // n~ = n+ik, as in M.D.Lay
    G4MaterialPropertyVector * _efficiency_photocathode; // necessary?
    G4MaterialPropertyVector * _thickness_photocathode; // function of Z (mm)
    G4MaterialPropertyVector * _angular_efficiency;
    G4MaterialPropertyVector * _angular_efficiency_err;
    // interior solid (vacuum), also initialized in constructor,
    // so we don't have to look it up each time DoIt is called.
    // Note it is implicitly assumed everywhere in the code that this solid
    // is vacuum-filled and placed in the body with no offset or rotation.

    // "luxury level" -- how fancy should the optical model be?
    G4int _luxlevel;
    G4int _electronProductionLevel = 3;

    // verbose level -- how verbose to be (diagnostics and such)
    G4int _verbosity;

    // directory for commands
    static G4UIdirectory* fgCmdDir;
    G4UIcmdWithAnInteger*      fVerbosityCmd;
    G4UIcmdWithAnInteger*      fLuxCmd;
    G4UIcmdWithAnInteger*      fElectrProdCmd;
    G4UIcmdWithADouble*        fElectrAbsLength;


    // "current values" of many parameters, for efficiency
    // [I claim it is quicker to access these than to
    // push them on the stack when calling CalculateCoefficients, Reflect, etc.]
    // The following are set by DoIt() prior to any CalculateCoefficients() call.
    G4double _photon_energy; // energy of current photon
    G4double _wavelength;    // wavelength of current photon
    G4double _n1;            // index of refraction of curr. medium at wavelength
    G4double _n2, _k2;       // index of refraction of photocathode at wavelength
    G4double _n3;            // index of refraction of far side at wavelength
    G4double _efficiency;    // efficiency of photocathode at wavelength (?)
    G4double _thickness;     // thickness of photocathode at current position
    G4double _cos_theta1;    // cosine of angle of incidence
    // The following are set by CalculateCoefficients()
    // and used by DoIt(), Refract(), and Reflect():
    G4double _sin_theta1;    // sine of angle of incidence
    G4double _sin_theta3;    // sine of angle of refraction
    G4double _cos_theta3;    // cosine of angle of refraction
    G4double fR_s;           // reflection coefficient for s-polarized light
    G4double fT_s;           // transmission coefficient for s-polarized light
    G4double fR_p;           // reflection coefficient for p-polarized light
    G4double fT_p;           // transmission coefficient for p-polarized light
    G4double fR_t;           // reflection coefficient total
    G4double fT_t;           // transmission coefficient total
    G4double fA_t;           // Absorption coefficient total
    G4double fR_n;           // reference for fR_s/p at normal incidence
    G4double fT_n;           // reference for fT_s/p at normal incidence
    G4double fA_n;

    G4double fPhotonAttenuationMu;
    G4double fElectronAttenuationMu = 2.85e4; //1/lambda (nm); lambda~35nm

    G4ThreeVector fNorm;

    TF1* ffMaclaurinSerie;

    // private methods
    G4VParticleChange* DoPhotocathodeInteraction (const G4Track &aTrack, const G4Step &aStep);
    bool LoadMaterialIndex (G4MaterialPropertiesTable* aMaterialPropertiesTable);
    bool LoadMaterialProps (G4MaterialPropertiesTable* aMaterialPropertiesTable);
    void InitPhotocathodeIndex ();

    void CalculateCoefficients(); // calculate and set fR_s, fR_p, fT_s and fT_p
    void CalculateTotalCoefficients (G4ThreeVector &dir, G4ThreeVector &pol, G4ThreeVector &norm);
    void Reflect(G4ThreeVector &dir, G4ThreeVector &pol, G4ThreeVector &norm);
    void Refract(G4ThreeVector &dir, G4ThreeVector &pol, G4ThreeVector &norm);
    unsigned char CheckDetection (const G4ThreeVector&);

    bool ElectronProductionCalculator (G4ThreeVector &dir, G4ThreeVector &pos);

    //public utils
  public:
    static bool IsAPhotocathode (const G4Step *STEP);
    static bool IsAPhotocathode (const G4VPhysicalVolume* preVol, const G4VPhysicalVolume*  postVol);

#ifdef ENABLE_TESTING
  private:
    struct ElectrProdData {
      float PhotonAttenuationMu;
      float LocalPhotocathodeThickness;
      float PhotoCathodeLength;
      float PhotonPenetrationProb;
      float probMax;
      float Rand;
      float ElectronLengthGeneration;
      float ElectronAbsLength;
      char  HasGoneInside;
      char  HasBeenGenerated;
      char  GoingInside;
};
    ElectrProdData fEPD;
#endif


  };
}
#endif

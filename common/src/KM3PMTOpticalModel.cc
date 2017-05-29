/** @file KM3PMTOpticalModel.cc
    Defines a FaStsimulationmodel- class for handling optical photon
    interactions with PMT: partial reflection, transmission, absorption,
    and hit generation.

    This file is part of the GenericLAND software library.
    $Id: KM3PMTOpticalModel.cc,v 1.7 2005/05/05 20:13:32 KM3sim Exp $

    @author Glenn Horton-Smith, March 20, 2001.
    @author Dario Motta, Feb. 23 2005: Formalism light interaction with photocathode.
*/
#include <TF1.h>

#include "KM3PMTOpticalModel.hh"
//#include <KM3PhysicsList.hh>
//#include "KM3PMTSD.hh"
#include <G4GeometryTolerance.hh>
#include <G4ParallelWorldProcess.hh>
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4TransportationManager.hh"
#include "Randomize.hh"
#include "G4UIcommand.hh"
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include "G4UIdirectory.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include "G4FastSimulationManager.hh"
#include <G4Electron.hh>
#include <G4DynamicParticle.hh>



//#include <G4FastSimulationManagerProcess.hh>

//#include "lf8ocal_g4compat.hh"
#include <complex.h>

#include <iostream>

using namespace std;

namespace km3net
{

  G4UIdirectory* KM3PMTOpticalModel::fgCmdDir = NULL;



  // constructor -- also handles all initialization
  KM3PMTOpticalModel::KM3PMTOpticalModel ()
  //    : G4VFastSimulationModel(modelName, photoregion, false),
  //    G4OpBoundaryProcess ()
  {
    _luxlevel= 3;
    _verbosity= 0;

    ffMaclaurinSerie = new TF1 ("MaclaurinSerie","TMath::Exp(-[0]*[0]/x)*(1-12304/22680.*[0]*[0]/x+5940/22680.*pow([0],4)/x/x-1380/22680.*pow([0],6)/pow(x,3)+105/22680.*pow([0],8)/pow(x,4))",0.,1e-3);

    // add UI commands
    if ( fgCmdDir == NULL ) {
      fgCmdDir = new G4UIdirectory("/PMTOpticalModel/");
      fgCmdDir->SetGuidance("PMT optical model control.");

      fVerbosityCmd= new G4UIcmdWithAnInteger("/PMTOpticalModel/verbose",this);
      fVerbosityCmd->SetGuidance("Set verbose level\n"
                                 " 0 == quiet\n"
                                 " 1 == minimal entrance/exit info\n"
                                 " 2 == +print verbose tracking info\n"
                                 " >= 10  +lots of info on thin photocathode calcs\n");
      fVerbosityCmd->SetParameter(new G4UIparameter("level", 'i', false));
      fVerbosityCmd->AvailableForStates(G4State_PreInit);

      fLuxCmd= new G4UIcmdWithAnInteger("/PMTOpticalModel/luxlevel",this);
      fLuxCmd->SetGuidance("Set \"luxury level\" for PMT Optical Model\n"
                           " 0 == standard \"black bucket\": photons stop in PC, maybe make pe, \n"
                           " 1 == shiny translucent brown film: photons only stop if they make a PE, otherwise 50/50 chance of reflecting/transmitting\n"
                           " 2 or greater == full model\n"
                           "The default value is 3."
                           );
      fLuxCmd->SetParameter(new G4UIparameter("level", 'i', false));
      fLuxCmd->AvailableForStates(G4State_PreInit);


      fElectrProdCmd= new G4UIcmdWithAnInteger("/PMTOpticalModel/ElectrProdlevel",this);
      fElectrProdCmd->SetGuidance("Set \"ElectrProduction level\" for PMT Optical Model\n"
                                  " 0 == basic, just a probability factor \n"
                                  " 1 == uses the thickness and electron abs length to get the prob. to produce a pe\n"
                                  );
      fElectrProdCmd->AvailableForStates(G4State_PreInit);


      fElectrAbsLength= new G4UIcmdWithADouble("/PMTOpticalModel/ElectrAbsLength",this);
      fElectrAbsLength->SetGuidance("Set electron absorption length in the photocathode. Should be set after \"ElectrProduction level\".");
      fElectrAbsLength->AvailableForStates(G4State_PreInit);

      fElectronProbLevel0Cmd= new G4UIcmdWithADouble("/PMTOpticalModel/ElectronProbLevel0",this);
      fElectronProbLevel0Cmd->AvailableForStates(G4State_PreInit);

    }

  }

  void KM3PMTOpticalModel::SetNewValue (G4UIcommand* command,G4String newValue)
  {
    if (command == fVerbosityCmd) {
      _verbosity = fVerbosityCmd->GetNewIntValue(newValue);
    }
    else if (command == fLuxCmd) {
      _luxlevel=fLuxCmd->GetNewIntValue(newValue);
    }
    else if (command == fElectrProdCmd) {
      _electronProductionLevel = fElectrProdCmd->GetNewIntValue (newValue);
    }
    else if (command == fElectrAbsLength) {
      fElectronAttenuationLength = fElectrAbsLength->GetNewDoubleValue (newValue);
    }
    else if (command == fElectronProbLevel0Cmd) {
      fElectronProbLevel0 = fElectronProbLevel0Cmd->GetNewDoubleValue (newValue);
    }


  }
  // destructor
  KM3PMTOpticalModel::~KM3PMTOpticalModel ()
  {
    delete fLuxCmd;
    delete fVerbosityCmd;
    delete fElectrProdCmd;
    delete fElectrAbsLength;
    delete fElectronProbLevel0Cmd;
    // nothing else to delete
    // Note: The "MaterialPropertyVector"s are owned by the material, not us.
  }



  bool KM3PMTOpticalModel::LoadMaterialIndex (G4MaterialPropertiesTable* aMaterialPropertiesTable)
  {
    if (aMaterialPropertiesTable == NULL)
      return false;
    G4MaterialPropertyVector* index = aMaterialPropertiesTable->GetProperty("RINDEX");
    if (index == NULL)
      {
        return false;
      }
    if (_rindexPre == NULL)
      {
        _rindexPre=index;
      }
    else
      {
        _rindexPost=index;
      }
    return true;
  }

  bool KM3PMTOpticalModel::LoadMaterialProps (G4MaterialPropertiesTable* aMaterialPropertiesTable)
  {
    if (!aMaterialPropertiesTable) {
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

    _kindex_photocathode= aMaterialPropertiesTable->GetProperty("KINDEX");
    if (_kindex_photocathode == NULL)
      {
        return false;
      }

    _rindex_photocathode= aMaterialPropertiesTable->GetProperty("RINDEX");
    if (_rindex_photocathode == NULL)
      {
        return false;
      }
    return true;

  }

  void KM3PMTOpticalModel::InitPhotocathodeIndex ()
  {
    _n1= _rindexPre->Value( _photon_energy );
    _n2= _rindex_photocathode->Value( _photon_energy );
    _k2= _kindex_photocathode->Value( _photon_energy );
    _n3= _rindexPost->Value(_photon_energy);
    _efficiency= _efficiency_photocathode->Value( _photon_energy );
    if (_verbosity > 0) cout << "efficienza " << _efficiency << endl;
  }

  G4VParticleChange* KM3PMTOpticalModel::PostStepDoIt (const G4Track &aTrack, const G4Step &aStep)
  {
    // Get hyperStep from  G4ParallelWorldProcess
    //  NOTE: PostSetpDoIt of this process should be
    //        invoked after G4ParallelWorldProcess!

    // Don't exectute anything if the step is too short
    if (aTrack.GetStepLength()<=G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/2){
      return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    const G4Step* pStep = &aStep;

    const G4Step* hStep = G4ParallelWorldProcess::GetHyperStep();

    if (hStep) pStep = hStep;

    /*
      Check if we are on boundary, then if it exist the material with the right properties.
    */

    G4bool isOnBoundary =
      (pStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary);

    G4Material* PreMaterial;
    G4Material* PostMaterial;

    if (isOnBoundary) {
      PreMaterial = pStep->GetPreStepPoint()->GetMaterial();
      PostMaterial = pStep->GetPostStepPoint()->GetMaterial();
    } else {
      return G4OpBoundaryProcess::PostStepDoIt(aTrack, aStep);
    }

    G4MaterialPropertiesTable* aMaterialPropertiesTable;

    aMaterialPropertiesTable = PreMaterial->GetMaterialPropertiesTable();
    if (!LoadMaterialIndex (aMaterialPropertiesTable))
      return G4OpBoundaryProcess::PostStepDoIt (aTrack, aStep);

    aMaterialPropertiesTable = PostMaterial->GetMaterialPropertiesTable();
    if (!LoadMaterialIndex (aMaterialPropertiesTable))
      return G4OpBoundaryProcess::PostStepDoIt (aTrack, aStep);

    G4VPhysicalVolume* thePrePV  =
      pStep->GetPreStepPoint() ->GetPhysicalVolume();
    G4VPhysicalVolume* thePostPV =
      pStep->GetPostStepPoint()->GetPhysicalVolume();

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
      return G4OpBoundaryProcess::PostStepDoIt (aTrack, aStep);

    return DoPhotocathodeInteraction (aTrack, aStep);



  }

  G4VParticleChange* KM3PMTOpticalModel::DoPhotocathodeInteraction (const G4Track &aTrack, const G4Step &aStep)
  {

    aParticleChange.Initialize(aTrack);
    //aParticleChange.ProposeVelocity(aTrack.GetVelocity());

    const G4AffineTransform GlobalToLocalTransform=
      aStep.GetPostStepPoint()->GetTouchable()->GetHistory()->GetTopTransform();
    const G4AffineTransform LocalToGlobalTransform=
      GlobalToLocalTransform.Inverse ();
    G4ThreeVector pos = aStep.GetPreStepPoint ()->GetPosition ();
    pos = GlobalToLocalTransform.TransformPoint(pos);
    G4ThreeVector dir = GlobalToLocalTransform.TransformAxis(aTrack.GetMomentumDirection());
    G4ThreeVector pol = GlobalToLocalTransform.TransformAxis(aTrack.GetPolarization());

    fNorm = -(aStep.GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume ()->GetSolid ()->SurfaceNormal( pos ));
    G4ThreeVector norm = fNorm;

    if (aStep.GetPreStepPoint()->
        GetMaterial()->
        GetMaterialPropertiesTable ()->
        ConstPropertyExists("electronSensitive"))
      fInsideDirection = -norm;
    else if (aStep.GetPostStepPoint()->
             GetMaterial()->
             GetMaterialPropertiesTable ()->
             ConstPropertyExists("electronSensitive"))
      fInsideDirection = norm;
    else
      fInsideDirection = G4ThreeVector (0,0,0);


    double energy = aTrack.GetKineticEnergy ();

    if ( energy != _photon_energy ) // equal to last energy?
      {
        _photon_energy= energy;
        _wavelength= twopi*hbarc / energy;
        InitPhotocathodeIndex ();
      }

    // set _thickness and _cos_theta1
    _thickness=  _thickness_photocathode->Value( pos.theta() );
  reflectphcth:
    _cos_theta1 = dir.cosTheta (norm);

    CalculateCoefficients ();
    CalculateTotalCoefficients (norm, pol, dir);

    if ( G4UniformRand() < fA_t)
      {
        cout << "ici " << fA_t << endl;

        aParticleChange.ProposeLocalEnergyDeposit(_photon_energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        if(ElectronProductionCalculator (dir, pol, norm))
          {
            G4DynamicParticle* anElectron= new G4DynamicParticle ( G4Electron::Electron (),
                                                                   fInsideDirection,
                                                                   2*eV      );
            aParticleChange.SetNumberOfSecondaries(1);
            aParticleChange.AddSecondary(anElectron);
          }
        return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
      }
    if (G4UniformRand() < fR_t/(fR_t+fT_t))
      {
        std::swap (_n1,_n3);
        Reflect (dir, pol, norm);
        dir.unit ();
        pol.unit ();
        //test old wrong method
        //norm=-norm;
        //goto reflectphcth;

        aParticleChange.ProposeMomentumDirection (LocalToGlobalTransform.TransformAxis(dir));
        aParticleChange.ProposePolarization (LocalToGlobalTransform.TransformAxis(pol));
        return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
      }
    else
      {
        Refract (dir, pol, norm);
        aParticleChange.ProposeMomentumDirection (LocalToGlobalTransform.TransformAxis(dir));
        aParticleChange.ProposePolarization (LocalToGlobalTransform.TransformAxis(pol));
        return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
      }
  }

  // CalculateCoefficients() method used by DoIt() above.
  // *** THE PHYSICS, AT LAST!!! :-) ***
  // Correct formalism implemented by Dario Motta (CEA-Saclay) 23 Feb 2005

  // declare the prototypes of some useful functions
  G4complex carcsin(G4complex theta); //complex sin^-1
  G4complex gfunc(G4complex ni, G4complex nj, G4complex ti, G4complex tj);
  G4complex rfunc(G4complex ni, G4complex nj, G4complex ti, G4complex tj);
  G4complex trfunc(G4complex ni, G4complex nj, G4complex ti, G4complex tj,
                   G4complex tk);

  double KM3PMTOpticalModel::GetTotalMeanPhotonAbsorbtionDepth (const G4ThreeVector &dir, const G4ThreeVector &pol, const G4ThreeVector &norm) const
  {
    double Mean_s=GetMeanPhotonAbsorbtionDepth(fr12_s,fr23_s,ft12_s,true);
    double Mean_p=GetMeanPhotonAbsorbtionDepth(fr12_p,fr23_p,ft12_p,false);

    G4double E_s2;
    // Calculate Transmission, Reflection, and Absorption coefficients
    if ( _sin_theta1 > 0.0 )
      {
        E_s2= ( pol * dir.cross( norm ) ) / _sin_theta1;
        E_s2*= E_s2;
      }
    else
      E_s2= 0.0;

    return Mean_s * E_s2  +  Mean_p * (1.0-E_s2);

  }

  double KM3PMTOpticalModel::GetMeanPhotonAbsorbtionDepth(G4complex r12,G4complex r23, G4complex t12, bool snotp) const
  {
    /*
     * Analytical absorption calculation
     * Assumptions:
     * n = _n2 + i_k2  (complex index)
     * k = k_T + i*2pi/lambda (complex wavenumber)
     * E = E0*exp(i(k_T + i*z/2*lambda) - wave propagation (z is always vertical, so lambda is "vertical lambda" = lambda*costheta
     * p_absorb = -dS*z/dz (Sz is the power flow), see 1603.02720
     * P_absorb_total = int_0^h p_absorb
     */
    //Step 1: amplitudes of two waves (up and down inside the photocathode)
    G4complex zi(0.,1.); //imaginary unit
    complex<double> ampv=t12/(1.+r12*r23*exp(2.*zi*fdelta));                          //Amplitude of exp(ikz) wave (going from the border with glass to the border with vac) at z=0 (border with glass)
    complex<double> ampw=t12*r23*exp(2.*zi*fdelta)/(1.+r12*r23*exp(2.*zi*fdelta));    //Amplitude of exp(ik(h-z)) wave (going in opposite direction) at z=0
    cout << "reflections " << conj(r23)*r23 << " " << conj(-r12)*(-r12) << endl;
    /*
     * k_Z=2*Pi*n*cos(theta)/wavelength=k_Transmission + ik_Absorption    z projection of k vector
     * in general n and cos(theta) are complex!
     * absorption length = 1/(2*k_Absorption) as it comes from Amp = E*conj(E)
     */
    G4complex _n2comp(_n2,_k2);
    complex<double> kz = twopi/_wavelength*_n2comp*cos(ftheta2);
    double VertLambdaPhoton                =_wavelength/(2.*twopi*imag(_n2comp*cos(ftheta2)));
    double k_T = twopi/_wavelength*real(_n2comp*cos(ftheta2));
    double A1, A2;
    complex<double> A3;
    if (snotp) {
      A1 = imag(_n2comp*cos(ftheta2)*kz)/real(_n1*cos(ftheta1))*real(ampw*conj(ampw));
      A2 = imag(_n2comp*cos(ftheta2)*kz)/real(_n1*cos(ftheta1))*real(ampv*conj(ampv));
      A3 = imag(_n2comp*cos(ftheta2)*kz)/real(_n1*cos(ftheta1))*ampv*conj(ampw);
    } else {
      A1 = 2.*imag(kz)*real(_n2comp*cos(conj(ftheta2)))/real(_n1*cos(conj(ftheta1)))*real(ampw*conj(ampw));
      A2 = 2.*imag(kz)*real(_n2comp*cos(conj(ftheta2)))/real(_n1*cos(conj(ftheta1)))*real(ampv*conj(ampv));
      A3 = 2.*real(kz)*imag(_n2comp*cos(conj(ftheta2)))/real(_n1*cos(conj(ftheta1)))*ampv*conj(ampw);
    }
    double A3_abs = abs(A3);
    double A3_phase = arg(A3);

    /*
     * check for fA - not needed in principle since it can be calculated as 1-fT-fR
     */
    double Integral1 = VertLambdaPhoton*(exp(_thickness/VertLambdaPhoton)-1.);
    double Integral2 = VertLambdaPhoton*(1.-exp(-_thickness/VertLambdaPhoton));
    double Integral3 = (sin(2.*k_T*_thickness+A3_phase)-sin(A3_phase))/2./k_T;
    double fA = A1*Integral1+A2*Integral2+2.*A3_abs*Integral3;

    /*
     * Probability calculations to check the formulas in 1603.02720.
     * Not needed in the final calculation.
     */
    double probtot = 0;
    double meandepthapprox = 0;
    double step = _thickness/1e6;
    for (double z = 0; z <= _thickness; z+=step) {
      double prob = A1*exp(2.*z*imag(kz))+A2*exp(-2.*z*imag(kz))+2.*A3_abs*cos(2.*z*k_T+A3_phase);
      //check of formulas (24) in 1603.02720
      /*
        complex<double> Ef = ampv*exp(zi*kz*z);
        complex<double> Eb = ampw*exp(-zi*kz*z);
        double prob_s = norm(Ef+Eb)*imag(_n2comp*cos(ftheta2)*kz)/real(_n1*cos(conj(ftheta1)));
        double prob_p = imag(_n2comp*cos(conj(ftheta2))*(kz*norm(Ef+Eb)-conj(kz)*norm(Ef-Eb)))/real(_n1*cos(conj(ftheta1))); //this one is "-" of the (24) for the p-polarisation in 1603.02720
        cout << "probs: " << prob << " " << prob_s << " " << prob_p << endl;
      */
      probtot+=prob*step;
      meandepthapprox+=prob*z*step;
    }
    cout << "ici totp = " << fA << " " << probtot << endl;

    double XIntegral1 = VertLambdaPhoton*_thickness*exp(_thickness/VertLambdaPhoton)+VertLambdaPhoton*VertLambdaPhoton*(1.-exp(_thickness/VertLambdaPhoton));
    double XIntegral2 = -VertLambdaPhoton*_thickness*exp(-_thickness/VertLambdaPhoton)+VertLambdaPhoton*VertLambdaPhoton*(1.-exp(-_thickness/VertLambdaPhoton));
    double XIntegral3 = _thickness*sin(2*k_T*_thickness + A3_phase)/2./k_T + (cos(2.*k_T*_thickness+A3_phase)-cos(A3_phase))/4./k_T/k_T;
    double MeanDepth = A1*XIntegral1+A2*XIntegral2+2.*A3_abs*XIntegral3;
    //cout << "meandepth " << MeanDepth << " " << meandepthapprox << endl;

    return MeanDepth/fA/_thickness;
  }

  bool KM3PMTOpticalModel::ElectronProductionCalculator (const G4ThreeVector &dir, const G4ThreeVector &pol, const G4ThreeVector &norm) const
  {
    if (_electronProductionLevel == 0 || fInsideDirection.r() == 0)
      {
        double rand=G4UniformRand ();
        return (rand < fElectronProbLevel0);
      }
    bool GoingInside                  = dir.dot (fInsideDirection) > 0;
    double PhotonDepth = GetTotalMeanPhotonAbsorbtionDepth (dir, pol, norm);
    /*
     * electron probability to escape in the proper media is proportional to the distance to the wrong media
     * (to the glass/incoming medium in case of the photon coming from the glass)
     */
    double Rand = G4UniformRand ();
    bool HasGoneInside = ((Rand < PhotonDepth && GoingInside) ||
                          (Rand > PhotonDepth && !GoingInside));

    //considering the survival probability of the photo-electron in the photocathode linked to its distance to the vacuum (exponential distribution)
    if (HasGoneInside)
      {
        double LocalPhotocathodeThickness = _thickness;
        double Electron2Vacuum;
        Electron2Vacuum  = (1-PhotonDepth)*LocalPhotocathodeThickness;


        double probMax = exp(-Electron2Vacuum/fElectronAttenuationLength);
        Rand = G4UniformRand ();

        return Rand < probMax;
      }
    return false;

  }

  void
  KM3PMTOpticalModel::CalculateCoefficients()
  // calculate and set fR_s, etc.
  {
    if (_luxlevel <= 0) {
      // no reflection or transmission, just a black "light bucket"
      // 100% absorption, and QE will be renormalized later
      fR_s= fR_p= 0.0;
      fT_s= fT_p= 0.0;
      fR_n= 0.0;
      fT_n= 0.0;
      return;
    }
    else if (_luxlevel == 1) {
      // this is what was calculated before, when we had no good defaults
      // for cathode thickness and complex rindex
      // set normal incidence coefficients: 50/50 refl/transm if not absorb.
      fR_n= fT_n= 0.5*(1.0 - _efficiency);
      // set sines and cosines
      _sin_theta1= sqrt(1.0-_cos_theta1*_cos_theta1);
      _sin_theta3= _n1/_n3 * _sin_theta1;
      if (_sin_theta3 > 1.0) {
        // total non-transmission -- what to do?
        // total reflection or absorption
        _cos_theta3= 0.0;
        fR_s= fR_p= 1.0 - _efficiency;
        fT_s= fT_p= 0.0;
        return;
      }
      _cos_theta3= sqrt(1.0-_sin_theta3*_sin_theta3);
      fR_s= fR_p= fR_n;
      fT_s= fT_p= fT_n;
      return;
    }
    // else...


    // declare some useful constants
    //G4complex _n2comp(_n2,-_k2); //Complex photocathode refractive index. This is from "E=E_0 exp(-i(kz-wt))" notation used by electrical engineers. Phase change is exp(-i*2pi/lambda*n*h*costheta) or exp(-zi*delta) all-over in the code. This convention is used by Motta and Schonert in formulas (2-3) however they introduce n=n+ik by typo.
    G4complex _n2comp(_n2,_k2); //this is a "physicist" convention. E=E_0 exp(i(kz-wt)). Phase change is exp(i*2pi/lambda*n*h*costheta) or exp(zi*delta) all-over in the code. This convention is used in Born & Wolf. For more details see https://en.wikipedia.org/wiki/Mathematical_descriptions_of_opacity#Complex_conjugate_ambiguity.
    G4complex eta=twopi*_n2comp*_thickness/_wavelength;
    //convention n = n-ik assumes

    G4complex zi(0.,1.); //imaginary unit

    // declare local variables


    //G4complex r12,r23,t12,t21,t23;//reflection- and transmission-related terms
    G4complex ampr,ampt; //relfection and transmission amplitudes

    // first set sines and cosines
    _sin_theta1= sqrt(1.0-_cos_theta1*_cos_theta1);
    _sin_theta3= _n1/_n3 * _sin_theta1;
    if (_sin_theta3 > 1.0) {
      // total non-transmission -- what to do???
      // these variables only used to decide refracted track direction,
      // so doing the following should be okay:
      _sin_theta3= 1.0;
    }
    _cos_theta3= sqrt(1.0-_sin_theta3*_sin_theta3);

    // Determine all angles. Branch selection adjusted accoring to arxiv 1603.02720
    ftheta1=asin(_sin_theta1);//incidence angle
    ftheta2=carcsin((_n1/_n2comp)*_sin_theta1);//complex angle in the photocathode
    //D.2 case of 1603.02720
    if (imag(_n2comp*cos(ftheta2)) < 0) {
      cout << "WARNING: the branch was wrongly chosen for theta2. Adjusting it." << endl;
      ftheta2 = twopi/2. - ftheta2;
    }
    ftheta3=carcsin((_n2comp/_n3)*sin(ftheta2));//angle of refraction into vacuum
    //if (imag(ftheta3)<0.) ftheta3=conj(ftheta3);//needed! (sign ambiguity arcsin)
    /*
     * D.3 case of 1603.02720
     * n1*sintheta1 > n3  - internal reflection  (check Im(n3*costheta3 > 0)
     * n1*sintheta1 = n3  - boundary (costheta = 0, so we don't care)
     * n1*sintheta1 < n3  - ordinary ref+trans   (check n3*costheta3 > 0)
     */
    if ((_n1*_sin_theta1 > _n3 && imag(_n3*cos(ftheta3)) < 0) || (_n1*_sin_theta1 < _n3 && _n3*real(cos(ftheta3)) < 0)) {
      //cout << "WARNING: the branch was wrongly chosen for theta3. Adjusting it." << endl;
      ftheta3 = twopi/2. - ftheta3;
    }

    fdelta=eta*cos(ftheta2);

    //Calculation for the s-polarization

    fr12_s=rfunc(_n1,_n2comp,ftheta1,ftheta2);
    fr23_s=rfunc(_n2comp,_n3,ftheta2,ftheta3);
    ft12_s=trfunc(_n1,_n2comp,ftheta1,ftheta1,ftheta2);
    ft21_s=trfunc(_n2comp,_n1,ftheta2,ftheta2,ftheta1);
    ft23_s=trfunc(_n2comp,_n3,ftheta2,ftheta2,ftheta3);

    ampr=fr12_s+(ft12_s*ft21_s*fr23_s*exp(2.*zi*fdelta))/(1.+fr12_s*fr23_s*exp(2.*zi*fdelta));
    ampt=(ft12_s*ft23_s*exp(zi*fdelta))/(1.+fr12_s*fr23_s*exp(2.*zi*fdelta));

    //And finally...!
    fR_s=real(ampr*conj(ampr));
    //fT_s=real(gfunc(_n3,_n1,ftheta3,ftheta1)*ampt*conj(ampt));
    //in reality ftheta1 is real, so in future conj(ftheta1) can be omitted
    fT_s=real(_n3*cos(ftheta3))/real(_n1*cos(ftheta1))*real(ampt*conj(ampt));

    //Calculation for the p-polarization
    // In this convention r_p is positive when the incoming and reflected magnetic fields are anti-parallel, and negative when they are parallel. This convention has the convenient advantage that the s and p sign conventions are the same at normal incidence. This convention is used in Motta & Schoenert (but p and s-wave are swapped by typo, probably).
    fr12_p=rfunc(_n1,_n2comp,ftheta2,ftheta1);
    fr23_p=rfunc(_n2comp,_n3,ftheta3,ftheta2);

    //This is the convention from the https://en.wikipedia.org/wiki/Fresnel_equations and Born&Wolf 1.5.2 "Fresnel formulae" (cited by Motta and Shoenert). Both conventions bring the same results for fR_p and fT_p. For p polarization, a positive r or t means that the magnetic fields of the waves are parallel, while negative means anti-parallel. r=-r' between two conventions.
    //fr12_p=rfunc(_n2comp,_n1,theta1,theta2);
    //fr23_p=rfunc(_n3,_n2comp,theta2,theta3);

    ft12_p=trfunc(_n1,_n2comp,ftheta1,ftheta2,ftheta1);
    ft21_p=trfunc(_n2comp,_n1,ftheta2,ftheta1,ftheta2);
    ft23_p=trfunc(_n2comp,_n3,ftheta2,ftheta3,ftheta2);

    ampr=fr12_p+(ft12_p*ft21_p*fr23_p*exp(2.*zi*fdelta))/(1.+fr12_p*fr23_p*exp(2.*zi*fdelta));
    ampt=(ft12_p*ft23_p*exp(zi*fdelta))/(1.+fr12_p*fr23_p*exp(2.*zi*fdelta));

    //And finally...!
    fR_p=real(ampr*conj(ampr));
    //fT_p=real(gfunc(_n3,_n1,ftheta3,ftheta1)*ampt*conj(ampt));
    //in reality ftheta1 and ftheta3 are real, so in future real and conj(ftheta1) can be omitted
    fT_p=real(_n3*cos(conj(ftheta3)))/real(_n1*cos(conj(ftheta1)))*real(ampt*conj(ampt));

    //a proof that for p-wave gfunc is different in case of complex n -- Not needed in the final calc
    /*
      cout << real(fr12_s*conj(fr12_s)) + real(ft12_s*conj(ft12_s))*real(_n2comp*cos(ftheta2))/real(_n1*cos(ftheta1)) << endl;
      cout << real(fr12_p*conj(fr12_p)) + real(ft12_p*conj(ft12_p))*real(_n2comp*cos(conj(ftheta2)))/real(_n1*cos(conj(ftheta1))) << endl;
    */

# ifdef G4DEBUG
    if (_verbosity >= 10) {
      cout<<"=> lam, n1, n2: "<<_wavelength/nm<<" "<<_n1<<" "<<_n2comp<<endl;
      cout<<"=> Angles: "<<real(ftheta1)/degree<<" "<<ftheta2/degree<<" "
	  <<ftheta3/degree<<endl;
      cout<<"Rper, Rpar, Tper, Tpar: "<<fR_s<<" "<<fR_p<<" "<<fT_s<<" "<<fT_p;
      cout<<"\nRn, Tn : "<<fR_n<<" "<<fT_n;
      cout<<"\n-------------------------------------------------------"<<endl;
    }
# endif

  }

  void KM3PMTOpticalModel::CalculateTotalCoefficients (G4ThreeVector &dir, G4ThreeVector &pol, G4ThreeVector &norm)
  {
    G4double E_s2;
    // Calculate Transmission, Reflection, and Absorption coefficients
    if ( _sin_theta1 > 0.0 )
      {
        E_s2= ( pol * dir.cross( norm ) ) / _sin_theta1;
        E_s2*= E_s2;
      }
    else
      E_s2= 0.0;
    fT_t= fT_s * E_s2  +  fT_p * (1.0-E_s2);
    fR_t= fR_s * E_s2  +  fR_p * (1.0-E_s2);
    fA_t= 1.0 - (fT_t+fR_t);
  }

  double KM3PMTOpticalModel::CalculateNormalCoefficients ()
  {
    G4complex _n2comp(_n2,-_k2); //complex photocathode refractive index
    fdelta=twopi*_n2comp*_thickness/_wavelength;
    G4complex r12,r23,t12,t21,t23;//reflection- and transmission-related terms
    G4complex ampr,ampt; //relfection and transmission amplitudes
    G4complex zi(0.,1.); //imaginary unit

    //Calculation for both polarization (the same at normal incidence)
    r12=rfunc(_n1,_n2comp,0.,0.);
    r23=rfunc(_n2comp,_n3,0.,0.);
    t12=trfunc(_n1,_n2comp,0.,0.,0.);
    t21=trfunc(_n2comp,_n1,0.,0.,0.);
    t23=trfunc(_n2comp,_n3,0.,0.,0.);

    ampr=r12+(t12*t21*r23*exp(-2.*zi*fdelta))/(1.+r12*r23*exp(-2.*zi*fdelta));
    ampt=(t12*t23*exp(-zi*fdelta))/(1.+r12*r23*exp(-2.*zi*fdelta));

    //And finally...!
    fR_n=real(ampr*conj(ampr));
    fT_n=real(gfunc(_n3,_n1,0.,0.)*ampt*conj(ampt));

    return (fA_n= 1.0 - (fT_n+fR_n)); //The absorption at normal incidence
  }

  G4complex carcsin(G4complex theta) //complex sin^-1
  {
    G4complex zi(0.,1.);
    G4complex value=(1./zi)*(log(zi*theta+sqrt(1.-theta*theta)));
    return value;
  }

  G4complex gfunc(G4complex ni, G4complex nj, G4complex ti, G4complex tj)
  {
    G4complex value=(ni*cos(ti))/(nj*cos(tj));
    return value;
  }

  G4complex rfunc(G4complex ni, G4complex nj, G4complex ti, G4complex tj)
  {
    G4complex value=(ni*cos(ti)-nj*cos(tj))/(ni*cos(ti)+nj*cos(tj));
    return value;
  }

  G4complex trfunc(G4complex ni, G4complex nj, G4complex ti, G4complex tj,
                   G4complex tk)
  {
    G4complex value=2.*(ni*cos(ti))/(ni*cos(tj)+nj*cos(tk));
    return value;
  }


  // Reflect() method, used by DoIt()
  void
  KM3PMTOpticalModel::Reflect(G4ThreeVector &dir,
                              G4ThreeVector &pol,
                              G4ThreeVector &norm)
  {
    dir -= 2.*(dir*norm)*norm;
    pol -= 2.*(pol*norm)*norm;
  }

  // Refract() method, used by DoIt()
  void
  KM3PMTOpticalModel::Refract(G4ThreeVector &dir,
                              G4ThreeVector &pol,
                              G4ThreeVector &norm)
  {
    dir = (_cos_theta3 - _cos_theta1*_n1/_n3)*norm + (_n1/_n3)*dir;
    pol = (pol-(pol*dir)*dir).unit();
  }

  bool KM3PMTOpticalModel::IsAPhotocathode (const G4Step* STEP)
  {
    if (STEP->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
      return false;

    G4VPhysicalVolume* thePrePV  =
      STEP->GetPreStepPoint() ->GetPhysicalVolume();
    G4VPhysicalVolume* thePostPV =
      STEP->GetPostStepPoint()->GetPhysicalVolume();

    return IsAPhotocathode (thePrePV, thePostPV);
  }

  bool KM3PMTOpticalModel::IsAPhotocathode (const G4VPhysicalVolume* thePrePV, const G4VPhysicalVolume*  thePostPV)
  {
    G4LogicalSurface* Surface = G4LogicalBorderSurface::GetSurface(thePrePV, thePostPV);
    if (!Surface)
      Surface = G4LogicalBorderSurface::GetSurface(thePostPV, thePrePV);
    if (!Surface)
      Surface = G4LogicalBorderSurface::GetSurface(thePrePV, thePostPV);
    if (!Surface)
      Surface = G4LogicalSkinSurface::GetSurface(thePostPV->GetLogicalVolume ());
    if (!Surface)
      Surface = G4LogicalSkinSurface::GetSurface(thePrePV->GetLogicalVolume ());

    if (!Surface)
      return false;

    G4MaterialPropertiesTable* propTable=dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty())
      ->GetMaterialPropertiesTable();

    if (!propTable)
      {
        return false;
      }

    if (propTable->GetProperty("EFFICIENCY") == NULL )
      {
        return false;
      }

    if (propTable->GetProperty("THICKNESS") == NULL )
      {
        return false;
      }

    if (propTable->GetProperty("KINDEX") == NULL )
      {
        return false;
      }

    if (propTable->GetProperty("RINDEX") == NULL )
      {
        return false;
      }

    return true;


  }

}

/** @file KM3PMTOpticalModel.cc
    Defines a FastSimulationModel class for handling optical photon
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
      fVerbosityCmd->AvailableForStates(G4State_PreInit);


      fElectrProdCmd= new G4UIcmdWithAnInteger("/PMTOpticalModel/ElectrProdlevel",this);
      fElectrProdCmd->SetGuidance("Set \"ElectrProduction level\" for PMT Optical Model\n"
                                  " 0 == basic, just a probability factor \n"
                                  " 1 == uses the thickness and electron abs length to get the prob. to produce a pe\n"
                                  );
      fElectrProdCmd->SetParameter(new G4UIparameter("level", 'i', false));
      fVerbosityCmd->AvailableForStates(G4State_PreInit);


      fElectrAbsLength= new G4UIcmdWithADouble("/PMTOpticalModel/ElectrAbsLength",this);
      fElectrAbsLength->SetGuidance("Set electron absorption length in the photocathode. Should be set after \"ElectrProduction level\".");
      fVerbosityCmd->AvailableForStates(G4State_PreInit);

    }

  }

  void KM3PMTOpticalModel::SetNewValue (G4UIcommand* command,G4String newValue)
  {
    if (command == fVerbosityCmd) {
       _verbosity = fVerbosityCmd->GetNewIntValue(newValue);
     }
    if (command == fLuxCmd) {
      _luxlevel=fLuxCmd->GetNewIntValue(newValue);
    }
    if (command == fElectrProdCmd) {
      _electronProductionLevel = fElectrProdCmd->GetNewIntValue (newValue);
    }
    if (command == fElectrAbsLength) {
      fElectronAttenuationLength = fElectrAbsLength->GetNewDoubleValue (newValue);
    }

  }
  // destructor
  KM3PMTOpticalModel::~KM3PMTOpticalModel ()
  {
    delete fLuxCmd;
    delete fVerbosityCmd;
    delete fElectrProdCmd;
    delete fElectrAbsLength;
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
    double energy = aTrack.GetKineticEnergy ();

    if ( energy != _photon_energy ) // equal to last energy?
      {
        _photon_energy= energy;
        _wavelength= twopi*hbarc / energy;
        InitPhotocathodeIndex ();
      }

    // set _thickness and _cos_theta1
    _thickness=  _thickness_photocathode->Value( pos.theta() );

  gtPropagationInPh:
    _cos_theta1 = dir.cosTheta (norm);

    CalculateCoefficients ();
    CalculateTotalCoefficients (norm, pol, dir);
    fA_n= CalculateNormalCoefficients ();//1.0 - (fT_n+fR_n); //The absorption at normal incidence

    /*cout << aTrack.GetTrackID () << " " << aStep.GetPreStepPoint()->GetPhysicalVolume()->GetName () << " " <<
      aStep.GetPostStepPoint()->GetPhysicalVolume()->GetName () << endl;
      ((G4Track*)((void*)(&aTrack)))->SetTrackID (2);*/

    if ( G4UniformRand() < fA_t)
      {
        aParticleChange.ProposeLocalEnergyDeposit(_photon_energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        if(ElectronProductionCalculator (dir, pos))
          {
            G4DynamicParticle* anElectron= new G4DynamicParticle ( G4Electron::Electron (),
                                                                   -fNorm,
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

        norm=-norm;
        aParticleChange.ProposeMomentumDirection (LocalToGlobalTransform.TransformAxis(dir));
        aParticleChange.ProposePolarization (LocalToGlobalTransform.TransformAxis(pol));
        goto gtPropagationInPh;
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


  bool KM3PMTOpticalModel::ElectronProductionCalculator (G4ThreeVector &dir, G4ThreeVector &pos)
  {
    if (_electronProductionLevel == 0)
      {
        float rand=G4UniformRand ();
        return rand > 0.6;
      }

    bool GoingInside                  = dir.dot (fNorm) < 0;
    float AbsProb                     = (fA_t)/(1-fR_t); //we know it's absorbed, thus no reflection

    float lambdaPhoton                = -1/log(1-AbsProb);  //relative absorption length 
    float Rand                        = G4UniformRand ()*(AbsProb)+(1-AbsProb); //survival probability for the photon (no absorbtion probability)
    float PhotonDepth                 = 1+lambdaPhoton*log(Rand);               //relative (0 to 1) interaction depth of the photon from the destination medium (vacuum in case of the photon coming from the glass)
    float PhotonEscapingProb          = PhotonDepth;  //in case of the Browninan motion, the escaping probability is simply proportional to the electron distance from the destination medium

    Rand = G4UniformRand ();

    bool HasGoneInside = ((Rand < PhotonEscapingProb && GoingInside) ||
                          (Rand > PhotonEscapingProb && !GoingInside));

    //considering the survival probability of the photo-electron in the photocathode linked to its distance to the vacuum (exponential distribution)
    if (HasGoneInside)
      {
        float LocalPhotocathodeThickness = _thickness_photocathode->Value(pos.theta());
        float Electron2Vacuum  = (1-PhotonDepth)*LocalPhotocathodeThickness;

        float probMax = exp(-Electron2Vacuum/fElectronAttenuationLength);
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
    G4complex _n2comp(_n2,-_k2); //complex photocathode refractive index
    G4complex eta=twopi*_n2comp*_thickness/_wavelength;

    G4complex zi(0.,1.); //imaginary unit

    // declare local variables

    G4complex theta1,theta2,theta3,delta;//geometric parameters
    G4complex r12,r23,t12,t21,t23;//reflection- and transmission-related terms
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

    // Determine all angles
    theta1=asin(_sin_theta1);//incidence angle
    theta2=carcsin((_n1/_n2comp)*_sin_theta1);//complex angle in the photocathode
    theta3=carcsin((_n2comp/_n3)*sin(theta2));//angle of refraction into vacuum
    if (imag(theta3)<0.) theta3=conj(theta3);//needed! (sign ambiguity arcsin)

    delta=eta*cos(theta2);

    //Calculation for the s-polarization

    r12=rfunc(_n1,_n2comp,theta1,theta2);
    r23=rfunc(_n2comp,_n3,theta2,theta3);
    t12=trfunc(_n1,_n2comp,theta1,theta1,theta2);
    t21=trfunc(_n2comp,_n1,theta2,theta2,theta1);
    t23=trfunc(_n2comp,_n3,theta2,theta2,theta3);

    ampr=r12+(t12*t21*r23*exp(-2.*zi*delta))/(1.+r12*r23*exp(-2.*zi*delta));
    ampt=(t12*t23*exp(-zi*delta))/(1.+r12*r23*exp(-2.*zi*delta));

    //And finally...!
    fR_s=real(ampr*conj(ampr));
    fT_s=real(gfunc(_n3,_n1,theta3,theta1)*ampt*conj(ampt));

    //Calculation for the p-polarization

    r12=rfunc(_n1,_n2comp,theta2,theta1);
    r23=rfunc(_n2comp,_n3,theta3,theta2);
    t12=trfunc(_n1,_n2comp,theta1,theta2,theta1);
    t21=trfunc(_n2comp,_n1,theta2,theta1,theta2);
    t23=trfunc(_n2comp,_n3,theta2,theta3,theta2);

    ampr=r12+(t12*t21*r23*exp(-2.*zi*delta))/(1.+r12*r23*exp(-2.*zi*delta));
    ampt=(t12*t23*exp(-zi*delta))/(1.+r12*r23*exp(-2.*zi*delta));

    //And finally...!
    fR_p=real(ampr*conj(ampr));
    fT_p=real(gfunc(_n3,_n1,theta3,theta1)*ampt*conj(ampt));

# ifdef G4DEBUG
    if (_verbosity >= 10) {
      cout<<"=> lam, n1, n2: "<<_wavelength/nm<<" "<<_n1<<" "<<_n2comp<<endl;
      cout<<"=> Angles: "<<real(theta1)/degree<<" "<<theta2/degree<<" "
	  <<theta3/degree<<endl;
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
    G4complex delta=twopi*_n2comp*_thickness/_wavelength;
    G4complex r12,r23,t12,t21,t23;//reflection- and transmission-related terms
    G4complex ampr,ampt; //relfection and transmission amplitudes
    G4complex zi(0.,1.); //imaginary unit

    //Calculation for both polarization (the same at normal incidence)
    r12=rfunc(_n1,_n2comp,0.,0.);
    r23=rfunc(_n2comp,_n3,0.,0.);
    t12=trfunc(_n1,_n2comp,0.,0.,0.);
    t21=trfunc(_n2comp,_n1,0.,0.,0.);
    t23=trfunc(_n2comp,_n3,0.,0.,0.);

    ampr=r12+(t12*t21*r23*exp(-2.*zi*delta))/(1.+r12*r23*exp(-2.*zi*delta));
    ampt=(t12*t23*exp(-zi*delta))/(1.+r12*r23*exp(-2.*zi*delta));

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


  //deprecated
  unsigned char KM3PMTOpticalModel::CheckDetection(const G4ThreeVector& localPoint)
  {

    unsigned char detectedGamma = 0;

    double eta_E = _efficiency/fA_n;

    double teta = localPoint.theta ();//acos(costeta);

    double eta_teta=_angular_efficiency->Value(teta/3*2.1);
    double eta_teta_err=_angular_efficiency_err->Value(teta/3*2.1);

    //cout << eta_teta << ' ' << teta/3*2.1<<' ' << eta_E << endl;

    double rndm = G4UniformRand();
    if(rndm < eta_E)
      {
        rndm=G4UniformRand();
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
        else
          {
            detectedGamma = 4;
          }

      }

    return detectedGamma;
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

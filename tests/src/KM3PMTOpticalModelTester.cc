#include <TTree.h>
#include <TerminalHistogram.hh>
#include <TFile.h>

#include <KM3PMTOpticalModelTester.hh>
#include <iostream>
#include <limits>

using namespace std;

namespace km3net
{
  KM3PMTOpticalModelTester::KM3PMTOpticalModelTester ()
  {

  }

  bool KM3PMTOpticalModelTester::LoadMaterial (string PhMaterial, string OutMaterial, string InMaterial)
  {
    fOpticalModel._electronProductionLevel = 2;
    fGlassAbsLength = KM3Material::GetIt ()->GetMaterial(OutMaterial)->GetMaterialPropertiesTable()->GetProperty ("ABSLENGTH");
    if (!(fOpticalModel.LoadMaterialProps(KM3Material::GetIt ()->GetMaterial(PhMaterial)->GetMaterialPropertiesTable()) &&
          fOpticalModel.LoadMaterialIndex(KM3Material::GetIt ()->GetMaterial(OutMaterial)->GetMaterialPropertiesTable()) &&
          fOpticalModel.LoadMaterialIndex(KM3Material::GetIt ()->GetMaterial(InMaterial)->GetMaterialPropertiesTable())))
      {
        cerr << "KM3PMTOpticalModelTester:ERROR: the material " << PhMaterial << " isn't a photocathode or in and out materials are not correct:"<< endl;
        G4Material::GetMaterial(PhMaterial)->GetMaterialPropertiesTable()->DumpTable ();
        G4Material::GetMaterial(OutMaterial)->GetMaterialPropertiesTable()->DumpTable ();
        G4Material::GetMaterial(InMaterial)->GetMaterialPropertiesTable()->DumpTable ();
        return false;
      }

    return true;
  }

  float KM3PMTOpticalModelTester::TestCalculateCoefficients (G4ThreeVector &pos, G4ThreeVector &dir, G4ThreeVector &pol, G4ThreeVector &norm, float wavelength)
  {
    fOpticalModel._thickness = fOpticalModel._thickness_photocathode->Value( pos.theta() );
    fOpticalModel._wavelength = wavelength;
    fOpticalModel._photon_energy = twopi*hbarc/wavelength;
    fOpticalModel.InitPhotocathodeIndex ();
    fOpticalModel._cos_theta1 = dir.cosTheta (norm);
    fOpticalModel.CalculateCoefficients ();
    fOpticalModel.CalculateTotalCoefficients (pos, dir, norm);
    return fOpticalModel.fA_t;
  }

  float KM3PMTOpticalModelTester::TestCalculateNormalCoefficients (G4ThreeVector &pos, float wavelength)
  {
    fOpticalModel._thickness = fOpticalModel._thickness_photocathode->Value( pos.theta() );
    fOpticalModel._wavelength = wavelength;
    fOpticalModel._photon_energy = twopi*hbarc/wavelength;
    fOpticalModel.InitPhotocathodeIndex ();
    fOpticalModel.CalculateNormalCoefficients ();
    return fOpticalModel.fA_n;
  }

  void KM3PMTOpticalModelTester::QuantumEfficiencyMeasurement ()
  {
    TerminalHistogram anHisto ("h", "h", 45, 250, 700);
    TerminalHistogram *theThisto;
    if (!fObjectCol.count ("QuantumEfficiencyHisto") && fObjectCol["QuantumEfficiencyHisto"] == NULL)
      {
        theThisto = new TerminalHistogram ("QuantumEfficiencyHisto","Quantum efficiency;nm",45,250,700);
        fObjectCol["QuantumEfficiencyHisto"] = theThisto;
      }
    else
      theThisto = (TerminalHistogram *)(fObjectCol["QuantumEfficiencyHisto"]);

    G4ThreeVector pos(0,0,1);
    G4ThreeVector norm(0,0,-1);
    G4ThreeVector dir(0,0,-1);
    G4ThreeVector pol(0,0,1);

    for (float wavelength = 250; wavelength < 700 ; wavelength+=10)
      {
        float energy = twopi*hbarc/wavelength/nm;
        anHisto.Fill (wavelength, fOpticalModel._kindex_photocathode->Value (energy));
        theThisto->Fill (wavelength, GetQuantumEfficiency (pos,dir,pol,norm,wavelength));
      }
    theThisto->Draw ();
    anHisto.Draw ();
  }

  float KM3PMTOpticalModelTester::GetQuantumEfficiency (G4ThreeVector &pos, G4ThreeVector &dir, G4ThreeVector &pol, G4ThreeVector &norm, float wavelength)
  {
    float Abs1,Ref1,Abs2,Ref2,Elexit1=0, Elexit2=0;
    TestCalculateCoefficients (pos, dir, pol, norm, wavelength*nm);
    TestCalculateNormalCoefficients (pos, wavelength*nm);

    for (int elescape=0; elescape<fElPrecision; elescape++)
      {
        TestCalculateNormalCoefficients (pos, wavelength*nm);
        TestElectronProduction (dir, pos, norm);
        if (fOpticalModel.fEPD.HasBeenGenerated)
          Elexit1++;
      }
    Elexit1/=fElPrecision*1.1; //0.9 for collection efficiency

    Abs1=fOpticalModel.fA_t;
    Ref1=fOpticalModel.fR_t;
    dir=-dir;

    TestCalculateCoefficients (pos, dir, pol, norm, wavelength*nm);

    for (int elescape=0; elescape<fElPrecision; elescape++)
      {
        TestCalculateNormalCoefficients (pos, wavelength*nm);
        TestElectronProduction (dir, pos, norm);
        if (fOpticalModel.fEPD.HasBeenGenerated)
          Elexit2++;
      }
    Elexit2/=fElPrecision*1.1; //0.9 for collection efficiency
    Abs2=fOpticalModel.fA_t;
    Ref2=fOpticalModel.fR_t;

    dir=-dir;

    return (GetGlassAbsorption (wavelength, fGlassThickness)*
            (Elexit1*Abs1+
             Elexit2*Ref1*Abs2+
             Elexit1*Ref1*Abs2*Ref2*Abs1));

  }

  void KM3PMTOpticalModelTester::PhotoCathodeEfficiencyReader ()
  {
    TerminalHistogram *theThisto;
    if (!fObjectCol.count ("MaterialEfficiencyHisto") && fObjectCol["MaterialEfficiencyHisto"] == NULL)
      {
        theThisto = new TerminalHistogram ("MaterialEfficiencyHisto","Material efficiency;nm",45,250,700);
        fObjectCol["MaterialEfficiencyHisto"] = theThisto;
      }
    else
      theThisto = (TerminalHistogram *)(fObjectCol["MaterialEfficiencyHisto"]);

    for (float wavelength = 250; wavelength < 700 ; wavelength+=10)
      {
        float energy = twopi*hbarc/wavelength/nm;
        theThisto->Fill (wavelength,fOpticalModel._efficiency_photocathode->Value( energy ));
      }
    theThisto->Draw ();

  }

  void KM3PMTOpticalModelTester::TestElectronProduction (G4ThreeVector &dir, G4ThreeVector &pos, G4ThreeVector &norm)
  {

    TTree* theTree;
    if (!fObjectCol.count ("ElectrProdTree") && fObjectCol["ElectrProdTree"] == NULL)
      {
        theTree = new TTree ("ElectrProdTree", "Electron production testing tree");
        fObjectCol["ElectrProdTree"] = theTree;

        theTree->Branch ("data", &(fOpticalModel.fEPD),"\
PhotonAttenuationMu/F:\
LocalPhotocathodeThickness/F:\
PhotoCathodeLength/F:\
PhotonPenetrationLength/F:\
probMax/F:\
Rand/F:\
ElectronLengthGeneration/F:\
ElectronAbsLength/F:\
HasGoneInside/B:\
HasBeenGenerated/B:\
GoingInside/B");
      }
    else
      theTree = (TTree*)(fObjectCol["ElectrProdTree"]);


    memset(&(fOpticalModel.fEPD),0,sizeof(fOpticalModel.fEPD));

    fOpticalModel.fNorm = norm;
    if (fOpticalModel.ElectronProductionCalculator (dir, pos))
      fOpticalModel.fEPD.HasBeenGenerated = 10;
    theTree->Fill ();
  }

  float KM3PMTOpticalModelTester::GetEfficiency (float wavelength)
  {
    float energy = twopi*hbarc/wavelength/nm;
    return fOpticalModel._efficiency_photocathode->Value (energy);
  }

  float KM3PMTOpticalModelTester::GetGlassAbsorption (float wavelength, float thickness)
  {
    if (fGlassAbsLength == NULL)
      return 1.;
    float energy = twopi*hbarc/wavelength/nm;
    return (exp(-thickness/fGlassAbsLength->Value (energy)));
  }

  void KM3PMTOpticalModelTester::SaveTreesAndHistograms (string FileName)
  {
    TFile fout (FileName.data(), "RECREATE");
    for (auto &object: fObjectCol)
      if (object.second != NULL)
        {
          fout.Add (object.second);
        }
    fout.Write ();
    fout.Close ();
  }

}

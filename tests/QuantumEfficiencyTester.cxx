#include <TH1I.h>

#include <KM3PMTOpticalModelTester.hh>

#include <TerminalHistogram.hh>
#include <iostream>

using namespace std;
using namespace km3net;

KM3PMTOpticalModelTester theModelTester;

bool PercentTo (float percent, TH1* histo, float value, int binmin, int binmax)
{
  float total = 0;
  for (int bin = binmin; bin<binmax;bin++)
    total+=abs(histo->GetBinContent (bin)-value);

  total/=(binmax-binmin);
  cout << "atotal " << total << endl;
  return (total < percent);
}

char CompareQuantumEfficiency (float wavelength)
{
  G4ThreeVector pos(0,0,1);
  G4ThreeVector norm(0,0,-1);
  G4ThreeVector dir(0,0,-1);
  G4ThreeVector pol(0,0,1);

  float compareit (theModelTester.GetEfficiency(wavelength) -
                   theModelTester.GetQuantumEfficiency (pos,dir,pol, norm, wavelength));

  cout << "compareit " << compareit << endl;

  if (compareit + 0.01 < 0)
    return -1;
  if (compareit - 0.01 > 0)
    return 1;
  return 0;

}

int main (int argc, char** argv)
{
  bool calibrateQE = false;
  float glassThickness = 1*mm;

  for (int argIt = 1; argIt < argc; argIt++)
    {
      string sarg = argv[argIt];
      if (sarg == "-cal")
        calibrateQE = true;
      if (sarg == "-PhThick")
        {
          sarg = argv[++argIt];
          glassThickness = stof (sarg)*mm;
        }
    }

  cout << "Create model tester" <<endl;
  theModelTester.LoadMaterial ("photocathode3inches", "PMTGlass", "Vacuum");
  theModelTester.fGlassThickness =glassThickness;
  TerminalHistogram *measuredQE =0;TerminalHistogram *simulatedQE =0;

  do {
    if (measuredQE != 0)
      {
        measuredQE->Reset ();
        simulatedQE->Reset ();
      }
    theModelTester.QuantumEfficiencyMeasurement ();
    theModelTester.PhotoCathodeEfficiencyReader ();

    measuredQE  = (TerminalHistogram*)(theModelTester.GetTObject ("MaterialEfficiencyHisto"));
    simulatedQE = (TerminalHistogram*)(theModelTester.GetTObject ("QuantumEfficiencyHisto"));

    G4MaterialPropertyVector oldKindexVector = theModelTester.GetKIndex ();
    G4MaterialPropertyVector newKindexVector;
    cout << "wavelength kindex " << endl;
    for (int bin = 1; bin <= measuredQE->GetNbinsX (); bin++)
      {
        float newbin=0;
        if (measuredQE->GetBinContent (bin) != 0)
          newbin=simulatedQE->GetBinContent (bin)/measuredQE->GetBinContent (bin);
        //cout << newbin << endl;
        measuredQE->SetBinContent (bin, newbin);
        float wavelength = measuredQE->GetBinCenter (bin);
        float energy = twopi*hbarc/wavelength/nm;
        float newKindex = 0;

        newKindex = oldKindexVector.Value (energy);

        if (wavelength > 330 && newbin != 0)
          {
            if (newbin < 1.1)
              newKindex = newKindex*(1.1/newbin);
          }


        newKindexVector.InsertValues (energy, newKindex);

        cout <<wavelength << " " << newKindex << endl;
      }
    theModelTester.SetKIndex (newKindexVector);
    measuredQE->Draw ();
  }while ( !PercentTo (0.02,measuredQE, 1.1, 11, 42) && calibrateQE);



  return 0;
}

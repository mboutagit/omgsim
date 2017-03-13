#include <TH1I.h>

#include <KM3PMTOpticalModelTester.hh>

#include <G4ThreeVector.hh>
#include <G4DynamicParticle.hh>
#include <G4SauterGavrilaAngularDistribution.hh>
#include <G4OpticalPhoton.hh>

#include <iostream>

using namespace std;
using namespace km3net;

TH1I* h=new TH1I("h", "h", 100,0,3.15);

int main  (int argc, char** argv)
{
  string sOuputTree;
  float x=0;

  for (int argIt = 1; argIt < argc; argIt++)
    {
      string sarg = argv[argIt];
      if (sOuputTree == "")
        sOuputTree = sarg;

      if (sarg == "-angle")
        {
          sarg = argv[++argIt];
          float angle = stof (sarg)*deg;
          x= tan (angle);
        }
    }

  if (!sOuputTree.size ())
    {
      cerr << "needs a file.root as argument for the output" << endl;
      return 1;
    }

  KM3PMTOpticalModelTester theModelTester;
  cout << "Create model tester" <<endl;
  theModelTester.LoadMaterial ("photocathode10inches", "PMTGlass", "Vacuum");

  G4ThreeVector dir(0,x,-1);
  G4ThreeVector pos(0,0,-1);
  G4ThreeVector norm(0,0,1);

  theModelTester.TestCalculateNormalCoefficients (pos, 440*nm);
  for (int i=0; i<20000;i++)
    theModelTester.TestElectronProduction (dir, pos, norm);


  theModelTester.SaveTreesAndHistograms ("/scratch/hugon/out.root");


  return 0;

  //this part was to check the angular emission of the electron. Left for history ;p
  G4DynamicParticle DynPart(G4OpticalPhoton::Definition (), dir, 2*eV);
  G4SauterGavrilaAngularDistribution SGAnglular;

  for (int i=0; i<10000; i++)
    {
      G4ThreeVector PEemit = SGAnglular.SampleDirection (&DynPart);
      cout << PEemit.x () << " " << PEemit.y () << " " << PEemit.z () << " " << endl;
      h->Fill (acos (PEemit.z ()));
    }

  h->SaveAs ("/scratch/hugon/bah.root");

  return 0;
}

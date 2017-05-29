#include <TH1F.h>
#include <TFile.h>
#include <TRandom.h>


#include <KM3PMTOpticalModelTester.hh>

#include <G4ThreeVector.hh>
#include <G4DynamicParticle.hh>
#include <G4SauterGavrilaAngularDistribution.hh>
#include <G4OpticalPhoton.hh>

#include <iostream>

using namespace std;
using namespace km3net;

TH1F* hElectEscape=new TH1F("hElectEscape", "Electron escping probability;deg", 90,0,90);
TH1F* hPhotonAbs=new TH1F("hPhotonAbs", "Electron absorption probability;deg", 90,0,90);
TH1F* hPhotonRefl=new TH1F("hPhotonRefl", "Electron absorption probability;deg", 90,0,90);
TH1F* hPhotonTrans=new TH1F("hPhotonTrans", "Electron absorption probability;deg", 90,0,90);

TH1F* hPhotonAbsP=new TH1F("hPhotonAbsP", "Electron absorption probability;deg", 90,0,90);
TH1F* hPhotonReflP=new TH1F("hPhotonReflP", "Electron absorption probability;deg", 90,0,90);
TH1F* hPhotonTransP=new TH1F("hPhotonTransP", "Electron absorption probability;deg", 90,0,90);

TH1F* hPhotonAbsS=new TH1F("hPhotonAbsS", "Electron absorption probability;deg", 90,0,90);
TH1F* hPhotonReflS=new TH1F("hPhotonReflS", "Electron absorption probability;deg", 90,0,90);
TH1F* hPhotonTransS=new TH1F("hPhotonTransS", "Electron absorption probability;deg", 90,0,90);


TRandom TR;

void RandomPolarization (const G4ThreeVector &dir, G4ThreeVector &pol)
{
  G4ThreeVector test (0,0,1);
  pol.setRThetaPhi (1, TR.Uniform(0,twopi),TR.Uniform(0,twopi));
  pol=dir.cross (pol);
}

void PPolarization (const G4ThreeVector &dir, G4ThreeVector &pol)
{
  pol.set (0, 1, 0);
  pol=dir.cross (pol);
	pol.unit();
}
void SPolarization (const G4ThreeVector &dir, G4ThreeVector &pol)
{
  pol.set (1, 0, 0);
  pol=dir.cross (pol);
	cout << "pol not norm " << pol << endl;
	pol.unit();
	cout << "pol norm " << pol << endl;
	cout << "pol abs " << pol.r() << endl;
	pol/=pol.r();
	cout << "pol norm finally " << pol << endl;
}

void TPolarization (const G4ThreeVector &dir, G4ThreeVector &pol)
{
  pol.set (1, 1, 0);
  pol=dir.cross (pol);
}


int main  (int argc, char** argv)
{
  string sOutputTree;
  float x=0,z=-1;
  int level = 1;
  for (int argIt = 1; argIt < argc; argIt++)
    {
      string sarg = argv[argIt];
      if (sOutputTree == "")
        sOutputTree = sarg;

      else if (sarg == "-angle")
        {
          sarg = argv[++argIt];
          float angle = stof (sarg)*deg;
          x= sin (angle);
          z= -cos (angle);
        }
      else if (sarg == "-el")
        {
          sarg = argv[++argIt];
          level=stoi(sarg);
        }
    }

  if (!sOutputTree.size ())
    {
      cerr << "needs a file.root as argument for the output" << endl;
      return 1;
    }

  KM3PMTOpticalModelTester theModelTester;
  cout << "Create model tester" <<endl;
  theModelTester.LoadMaterial ("PhotoMotta", "PMTGlassMotta", "Vacuum");

  //VLA
  G4ThreeVector pos(0,0,-1);
  G4ThreeVector pol(0,0,1);
  theModelTester.TestCalculateNormalCoefficients (pos, 442*nm);
  theModelTester.SetElectronProductionLevel (level);

  theModelTester.SetElectronAbsorptionLength (220000*nm);


	double angle;
  for (int angle1=0;angle1<90;angle1++)
    {
			angle = sin(angle1*deg)*1.50/1.51;
			cout << "angle sin " << angle << endl;
			if (angle < 1) {
				angle = asin(angle)/deg;
				x= sin (angle*deg);
				z= -cos (angle*deg);
				G4ThreeVector dir(x,0,z);
				SPolarization (dir, pol);
				//pol.set(0,1,0);
				cout << "Spol " << pol << endl;
				//G4ThreeVector pos(0,0,-1);
				G4ThreeVector norm(0,0,1);
				G4ThreeVector insideDirection (0,0,-1);

				float escaped=0;
				float nb_elect=50000;
				theModelTester.TestCalculateCoefficients (pos, dir, pol, norm, 442*nm);
				double Abs,Trans,Refl;
				double AbsT,TransT,ReflT;
				theModelTester.GetTotalProbabilities (Abs, Trans, Refl);

				double F = pow(sin(angle1*deg-angle*deg)/sin(angle1*deg+angle*deg),2);
				cout << "fs " << F << endl;
				Trans= Trans*(1-F)/(1-F*Refl);
				Refl = F+Refl*(1.-F)*(1.-F)/(1-F*Refl);
				Abs = 1-Trans-Refl;
				Refl -= F;


				hPhotonAbsS->SetBinContent (angle1,Abs);
				hPhotonTransS->SetBinContent (angle1,Trans);
				hPhotonReflS->SetBinContent (angle1,Refl);


				AbsT=Abs; TransT=Trans; ReflT=Refl;
				PPolarization (dir, pol);
				cout << "Ppol " << pol << endl;
				theModelTester.TestCalculateCoefficients (pos, dir, pol, norm, 442*nm);
				theModelTester.GetTotalProbabilities (Abs, Trans, Refl);

				F = pow(tan(angle1*deg-angle*deg)/tan(angle1*deg+angle*deg),2);
        cout << "fs " << F << endl;
        Trans= Trans*(1-F)/(1-F*Refl);
        Refl = F+Refl*(1.-F)*(1.-F)/(1-F*Refl);
        Abs = 1-Trans-Refl;
        Refl -= F;

				hPhotonAbsP->SetBinContent (angle1,Abs);
				hPhotonTransP->SetBinContent (angle1,Trans);
				hPhotonReflP->SetBinContent (angle1,Refl);

				AbsT+=Abs; TransT+=Trans; ReflT+=Refl;
				hPhotonAbs->SetBinContent (angle1,AbsT/2);
				hPhotonTrans->SetBinContent (angle1,TransT/2);
				hPhotonRefl->SetBinContent (angle1,ReflT/2);
				float Pescaped = theModelTester.TestElectronEscapingProbability (dir, pol,norm);
				//cout << angle+1 << " " << Pescaped << endl;
				hElectEscape->SetBinContent (angle+1,Pescaped);
			}
    }

  //theModelTester.SaveTreesAndHistograms ("/scratch/hugon/out.root");
  TFile tf(sOutputTree.c_str(),"recreate");
  tf.Add (hElectEscape);
  tf.Add (hPhotonAbs);  tf.Add (hPhotonTrans);  tf.Add (hPhotonRefl);
  tf.Add (hPhotonAbsS);  tf.Add (hPhotonTransS);  tf.Add (hPhotonReflS);
  tf.Add (hPhotonAbsP);  tf.Add (hPhotonTransP);  tf.Add (hPhotonReflP);
  tf.Write ();
  tf.Close ();

  return 0;

  /*/this part was to check the angular emission of the electron. Left for history ;p
  //G4DynamicParticle DynPart(G4OpticalPhoton::Definition (), dir, 2*eV);
  G4SauterGavrilaAngularDistribution SGAnglular;

  for (int i=0; i<10000; i++)
  {
  G4ThreeVector PEemit = SGAnglular.SampleDirection (&DynPart);
  cout << PEemit.x () << " " << PEemit.y () << " " << PEemit.z () << " " << endl;
  h->Fill (acos (PEemit.z ()));
  }



  return 0;*/
}

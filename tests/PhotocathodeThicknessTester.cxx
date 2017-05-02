#include <TH1I.h>

#include <KM3PMTOpticalModelTester.hh>

#include <TerminalHistogram.hh>
#include <iostream>
#include "TGraph.h"
#include "TFile.h"



//to set the random engine
#include "Randomize.hh"
#include <sys/time.h>
#include <unistd.h>
#include <random>

using namespace std;
using namespace km3net;

KM3PMTOpticalModelTester theModelTester;

bool PercentTo (float percent, TH1* histo, float value, int binmin, int binmax)
{
	/*
  float total = 0;
  for (int bin = binmin; bin<binmax;bin++)
    total+=abs(histo->GetBinContent (bin)-value);

  total/=(binmax-binmin);
  cout << "atotal " << total << endl;
  return (total < percent);
	*/


  float worse = 0;
	float diff;
  for (int bin = binmin; bin<binmax;bin++) {
		diff = abs(histo->GetBinContent (bin)-value);
    if (worse>diff) worse= diff;
	}

  cout << "worse " << worse << endl;
  return (worse < percent);
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

	CLHEP::HepRandom::setTheEngine(new CLHEP::DualRand);
	//CLHEP::HepRandom::restoreEngineStatus();
	//G4int ttime = time(0);
	//G4cout << "ttime=" << ttime << G4endl;
	timeval tim;
	gettimeofday(&tim, NULL);
	long ttime = tim.tv_sec xor tim.tv_usec;
	G4cout << "ttime=" << ttime << G4endl;

	G4int pid = getpid();
	G4cout << "pid=" << pid << G4endl;

	long Seed = ttime xor (pid << 8);
	// automatic (time-based) random seeds and filenames for each run
	G4cout << "******************" << G4endl;
	G4cout << "*** AUTOSEED ON ***" << G4endl;
	G4cout << "*******************" << G4endl;
	long seeds[2];
	seeds[0] =  Seed;
	seeds[1] =  (long)(Seed*G4UniformRand());
	G4cout << "seed1: " << seeds[0] << "; seed2: " << seeds[1] << G4endl;

	CLHEP::HepRandom::getTheEngine()->setSeeds(seeds,-1);
	CLHEP::HepRandom::showEngineStatus();

  bool calibrateQE = false;
  float glassThickness = 1*mm;
	float precision = 0.01;

	theModelTester.LoadMaterial ("photocathode3inches", "PMTGlass_R12199_02", "Vacuum");
  theModelTester.fGlassThickness =glassThickness;
	theModelTester.SetElectronProductionLevel(3);
	
	theModelTester.fElPrecision = 25./(precision*precision);  //this is the number of simulated e-, precision is aprrox 5~sigma and sigma is sqrt(N_e)

  TerminalHistogram *measuredQE =0;TerminalHistogram *simulatedQE =0; TerminalHistogram *ratioQE = 0;


	for (double cathodethick = 20; cathodethick <= 30; cathodethick+=1) {
		G4MaterialPropertyVector newThicknessVector;
		newThicknessVector.InsertValues (0, cathodethick*1e-6);
		newThicknessVector.InsertValues (188, cathodethick*1e-6);

		cout << "cathodethick: " << cathodethick << endl;
		theModelTester.SetCathodeThickness(newThicknessVector);
		theModelTester.QECheck(500);
	}



  return 0;
}

#include <TTree.h>

#include <MottaAnalysisManager.hh>
#include <KM3ExperimentTypeMessenger.hh>

#include <iostream>

using namespace std;

namespace km3net
{
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  const bool MottaAnalysisManager::tmpMottaAnalysisManagerBuilderUseless
  = km3net::KM3ExperimentTypeMessenger::GetIt()->SetAnAnalysisMgr ("Motta", (km3net::MottaAnalysisManager::getIt));

  MottaAnalysisManager::MottaAnalysisManager ()
  {
  }

  MottaAnalysisManager::~MottaAnalysisManager () {}

  void MottaAnalysisManager::Init (string RF)
  {
    AAAnalysisManager::Init(RF);
    fTEvent->Branch ("TransAbsRef",&fTranAbsRef, "TransAbsRef/B");
  }

  void MottaAnalysisManager::AddDetectedParticule (const KM3TrackInformation* INFO, unsigned char LIM)
  {
    fTranAbsRef = 0;
    AAAnalysisManager::AddDetectedParticule(INFO, LIM);
  }

  void MottaAnalysisManager::AddReflectedOrTransmittedParticle (char TransRef)
  {
    fTranAbsRef = TransRef;

    if (fRFile == NULL)
      return;
    if (fFastAnalysisOnly)
      return;

    fTEvent->Fill ();
  }

  MottaAnalysisManager* MottaAnalysisManager::getIt ()
  {
    if (!fManager)
      fManager = new MottaAnalysisManager;
    return (MottaAnalysisManager*)fManager;
  }



}

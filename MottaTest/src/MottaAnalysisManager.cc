#include <MottaAnalysisManager.hh>
#include <KM3ExperimentTypeMessenger.hh>

#include <iostream>

using namespace std;

namespace km3net
{
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  const bool MottaAnalysisManager::tmpMottaAnalysisManagerBuilderUseless
  = km3net::KM3ExperimentTypeMessenger::GetIt()->SetAnAnalysisMgr ("Motta", (km3net::MottaAnalysisManager::getIt));

  MottaAnalysisManager* MottaAnalysisManager::getIt ()
  {
    cout << "getting motta analysis" << endl;
    if (!fManager)
      fManager = new MottaAnalysisManager;
    return (MottaAnalysisManager*)fManager;
  }


}

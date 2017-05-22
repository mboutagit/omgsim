#ifndef MottaAnalysisManager_h
#define MottaAnalysisManager_h

#include <AAAnalysisManager.hh>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

namespace km3net
{
  class MottaAnalysisManager : public AAAnalysisManager
  {

  private:
    static MottaAnalysisManager* getIt ();
    MottaAnalysisManager() {};
    ~MottaAnalysisManager() {};

    static const bool tmpMottaAnalysisManagerBuilderUseless;
  };

}
#endif

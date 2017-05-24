#ifndef MottaAnalysisManager_h
#define MottaAnalysisManager_h

#include <AAAnalysisManager.hh>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

namespace km3net
{
  class MottaAnalysisManager : public AAAnalysisManager
  {
  public:
    virtual void AddDetectedParticule (const KM3TrackInformation* INFO, unsigned char LIM);
    void AddReflectedOrTransmittedParticle (char TransRef);

    static MottaAnalysisManager* getIt ();
    virtual void Init(std::string RF="");
  private:
    MottaAnalysisManager();
    ~MottaAnalysisManager();

    static const bool tmpMottaAnalysisManagerBuilderUseless;

  protected:
    char fTranAbsRef = 0;

  };

}
#endif

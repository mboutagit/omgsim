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
    void WentToTheGlass (bool b=true) {fTouched = b;}
    static MottaAnalysisManager* getIt ();
    virtual void Init(std::string RF="");
    virtual void EndOfEvent ();
  private:
    MottaAnalysisManager();
    ~MottaAnalysisManager();

    static const bool tmpMottaAnalysisManagerBuilderUseless;

  protected:
    char fTranAbsRef = 0;
    bool fTouched = 0;
  };

}
#endif

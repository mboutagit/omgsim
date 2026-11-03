#ifndef BRunManager_hh
#define BRunManager_hh

#include <G4RunManager.hh>
// #include "KM3RunManager.hh"
#include "BEvent.hh"

// using namespace km3net;
namespace lactel
{
  class BRunManager : public G4RunManager
  {
  public:
    BRunManager ();
    virtual ~BRunManager ();

    // const BEvent* GetCurrentEvent();
    // static BRunManager* GetRunManager();

    // G4Event* GenerateEvent(G4int eventID);
  protected:
    BEvent* currentEvent;
 
  private:
    // static BRunManager* runManager;
    const BEvent* event_;

  };

}

#endif

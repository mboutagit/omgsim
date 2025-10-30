#ifndef BRunManager_hh
#define BRunManager_hh

#include <G4RunManager.hh>
#include "KM3RunManager.hh"
#include "BEvent.hh"

using namespace km3net;
namespace lactel
{
  class BRunManager : public KM3RunManager
  {
  public:
    BRunManager ();
    virtual ~BRunManager ();

  protected:
 
  private:

  };


}

#endif

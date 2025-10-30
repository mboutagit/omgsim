#ifndef BEventAction_h 
#define BEventAction_h 

#include "KM3EventAction.hh"
#include "BAnalysis.hh"
#include "BEvent.hh"

using namespace km3net;
namespace lactel
{
  class BEventAction : public KM3EventAction
  {
  private:
      /* data */
  public:
      BEventAction();
      virtual ~BEventAction();
      void BeginOfEventAction(const G4Event* anEvent);
      void EndOfEventAction(const G4Event* anEvent);

    private:
      /*--*/
  };

} // namespace lactel

#endif
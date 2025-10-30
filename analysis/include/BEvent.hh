#ifndef BEvent_hh 
#define BEvent_hh 

#include "G4Event.hh"
#include "BHit.hh"

namespace lactel
{
  class BEvent : public G4Event
  {
  private:
      /* data */
  public:
      BEvent();
      BEvent(G4int id);
      virtual ~BEvent();
      
      std::vector<BHit> getHitCollection();
      void addHit(BHit ahit);

    private:
      std::vector<BHit> hitCollection;
  };

} // namespace lactel

#endif
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
      BEvent(G4int eventID = 0);
      virtual ~BEvent();

      double totalCharge = 0.0;
      double timeSpread = 0.0;

      void SetTotalCharge(double q) { totalCharge = q; }
      double GetTotalCharge() const { return totalCharge; }

      void SetTimeSpread(double t) { timeSpread = t; }
      double GetTimeSpread() const { return timeSpread; }
      
      std::vector<BHit> getHitCollection();
      void addHit(BHit ahit);

    private:
      std::vector<BHit> hitCollection;
  };

} // namespace lactel

#endif
#ifndef BHit_hh
#define BHit_hh

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "globals.hh"

namespace lactel
{
   class BHit : public G4VHit {
    public:
        BHit();
        virtual ~BHit();

        BHit(const BHit&);
        const BHit& operator=(const BHit&);
        G4bool operator==(const BHit&) const;

        inline void SetEdep(G4double val) { edep = val; }
        inline G4double GetEdep() const { return edep; }

        inline void SetTime(G4double val) { time = val; }
        inline G4double GetTime() const { return time; }

        inline void SetPos(const G4ThreeVector& val) { pos = val; }
        inline G4ThreeVector GetPos() const { return pos; }

        void Print() override;

    private:
        G4double edep;
        G4double time;
        G4ThreeVector pos;
    };

    extern G4ThreadLocal G4Allocator<BHit>* BHitAllocator;
 
} // namespace lactel
#endif

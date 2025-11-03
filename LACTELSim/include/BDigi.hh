#ifndef BDigi_hh 
#define BDigi_hh 

#include "G4VDigi.hh"
#include "G4Allocator.hh"
#include "globals.hh"


// using namespace km3net;
namespace lactel
{
    class BDigi : public G4VDigi {
    public:
        BDigi();
        ~BDigi() override;

        BDigi(const BDigi&);
        const BDigi& operator=(const BDigi&);
        int operator==(const BDigi&) const;

        inline void SetEdep(G4double e) { edep = e; }
        inline G4double GetEdep() const { return edep; }

        void Draw() override {}
        void Print() override;

    private:
        G4double edep;
    };

    extern G4ThreadLocal G4Allocator<BDigi>* BDigiAllocator;

} // namespace lactel

#endif
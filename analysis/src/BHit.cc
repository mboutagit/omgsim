#include "BHit.hh"
#include "G4SystemOfUnits.hh"

namespace lactel
{
    G4ThreadLocal G4Allocator<BHit>* BHitAllocator = nullptr;

    BHit::BHit() : edep(0), time(0), pos(G4ThreeVector()) {}
    BHit::~BHit() {}

    BHit::BHit(const BHit& right) : G4VHit() {
        edep = right.edep;
        time = right.time;
        pos = right.pos;
    }

    const BHit& BHit::operator=(const BHit& right) {
        edep = right.edep;
        time = right.time;
        pos = right.pos;
        return *this;
    }

    G4bool BHit::operator==(const BHit& right) const {
        return (this == &right);
    }

    void BHit::Print() {
        G4cout << "BHit: E = " << edep / eV << " eV, t = " << time / ns << " ns, pos = " << pos << G4endl;
    }    
} // namespace lactel


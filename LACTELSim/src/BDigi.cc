#include "BDigi.hh"
#include "globals.hh"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;
// using namespace km3net;
namespace lactel
{
    G4ThreadLocal G4Allocator<BDigi>* BDigiAllocator = nullptr;

    BDigi::BDigi() : edep(0.) {}
    BDigi::~BDigi() {}

    BDigi::BDigi(const BDigi& right) : G4VDigi() {
        edep = right.edep;
    }

    const BDigi& BDigi::operator=(const BDigi& right) {
        edep = right.edep;
        return *this;
    }

    int BDigi::operator==(const BDigi& right) const {
        return (this == &right);
    }

    void BDigi::Print() {
        G4cout << "Digitized energy deposit: " << edep / keV << " keV" << G4endl;
    }

} // namespace lactel

#include "BDigitizerModule.hh"
#include "G4DigiManager.hh"
#include "G4THitsCollection.hh"
#include "BHit.hh" 
#include "G4SDManager.hh"

namespace lactel
{

    BDigitizerModule::BDigitizerModule(const G4String& name) : G4VDigitizerModule(name) {
        collectionName.push_back("MyDigiCollection");
    }

    BDigitizerModule::~BDigitizerModule() {}

    void BDigitizerModule::Digitize() {
        digiCollection = new G4TDigiCollection<BDigi>("BDigitizerModule", collectionName[0]);

        G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
        G4int hcID = digiMan->GetHitsCollectionID("BHitsCollection");

       // G4THitsCollection<BHit>* hitsCollection = static_cast<G4THitsCollection<BHit>*>(digiMan->GetHitsCollection(hcID));
        const G4THitsCollection<BHit>* hitsCollection = static_cast<const G4THitsCollection<BHit>*>(digiMan->GetHitsCollection(hcID));

        if (!hitsCollection) return;

        G4double totalEdep = 0.0;

        for (size_t i = 0; i < hitsCollection->entries(); ++i) {
            totalEdep += (*hitsCollection)[i]->GetEdep();
        }

        BDigi* digi = new BDigi();
        digi->SetEdep(totalEdep);
        digiCollection->insert(digi);

        StoreDigiCollection(digiCollection);
    }

} // namespace lactel
#ifndef BDigitizerModule_hh 
#define BDigitizerModule_hh 

#include "G4VDigitizerModule.hh"
#include "BDigi.hh"
#include "globals.hh"
#include "G4TDigiCollection.hh"

// using namespace km3net;
namespace lactel
{
    class BDigitizerModule : public G4VDigitizerModule {
    public:
        BDigitizerModule(const G4String& name);
        ~BDigitizerModule() override;

        void Digitize() override;

    private:
        //G4DigiCollection* digiCollection;
        G4TDigiCollection<BDigi>* digiCollection;
    };
    

} // namespace lactel
#endif

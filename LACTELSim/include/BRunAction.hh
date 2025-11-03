#ifndef BRunAction_h 
#define BRunAction_h 

#include "KM3RunAction.hh"
#include "BPrimaryGeneratorAction.hh"
#include "BDetectorConstruction.hh"
#include "BAnalysis.hh"


using namespace km3net;

namespace lactel
{
    class BRunAction : public KM3RunAction
    {
    private:
        /* data */
    public:
        BRunAction(/* args */);
        ~BRunAction();
        
        void BeginOfRunAction(const G4Run*);
        void EndOfRunAction(const G4Run*);
        
    };
} // namespace lactel

#endif
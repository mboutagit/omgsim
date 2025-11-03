#ifndef BSteppingAction_h 
#define BSteppingAction_h 

#include "KM3SteppingAction.hh"
#include "KM3ExperimentTypeMessenger.hh"
#include "G4Step.hh"

#include "KM3TrackInformation.hh"
#include "KM3PMTOpticalModel.hh"
#include "KM3AnalysisManager.hh"


using namespace km3net;
using namespace std;

namespace lactel{
    class BSteppingAction : public KM3SteppingAction
    {
    private:

        // static BSteppingAction* GetIt ();
        // static BSteppingAction* fInstance;
        static bool tmpBSteppingActionBuilder;

    public:
        BSteppingAction();
        virtual ~BSteppingAction();
        virtual void UserSteppingAction(const G4Step*);
        // unsigned char CheckDetection(const G4ThreeVector& localPoint, KM3TrackInformation* infos);
        // unsigned char CheckDetection (const G4Step* STEP, KM3TrackInformation* infos);
        // unsigned char CalculateDetection(const G4Step* STEP);

    };
} // namespace lactel

#endif
#include <KM3TrackingAction.hh>
#include <KM3TrackInformation.hh>
#include <G4TrackingManager.hh>
#include <G4Track.hh>
#include <G4TrackVector.hh>

namespace km3net
{
  void KM3TrackingAction::PreUserTrackingAction(const G4Track* aTrack) {;}
  void KM3TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
  {
		KM3TrackInformation* anInfo = new KM3TrackInformation(aTrack);
		//G4Track* theTrack = (G4Track*)aTrack;
		//theTrack->SetUserInformation(anInfo);

    G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
    if(secondaries)
      {
				
        //KM3TrackInformation* info = (KM3TrackInformation*)(aTrack->GetUserInformation());
        size_t nSeco = secondaries->size();
        if(nSeco>0)
          {
            for(size_t i=0;i<nSeco;i++)
              {
                KM3TrackInformation* infoNew = new KM3TrackInformation (anInfo);
                (*secondaries)[i]->SetUserInformation(infoNew);
              }
          }
      }
  }
}

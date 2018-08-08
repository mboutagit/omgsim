#include <KM3TrackingAction.hh>
#include <KM3TrackInformation.hh>
#include <G4TrackingManager.hh>
#include <G4Track.hh>
#include <G4TrackVector.hh>

namespace km3net
{
  KM3TrackingAction::KM3TrackingAction () {}
  KM3TrackingAction::~KM3TrackingAction () {}

  void KM3TrackingAction::PreUserTrackingAction(const G4Track* aTrack) {
    G4Track* theTrack = (G4Track*)aTrack;
    KM3TrackInformation* info = (KM3TrackInformation*)(theTrack->GetUserInformation());

    if (info == 0)
      {
        info = new KM3TrackInformation(aTrack);
        theTrack ->SetUserInformation(info);
      }
    info->SetTrack (aTrack);
    info->SetStep (aTrack->GetStep ());

  }
  void KM3TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
  {

    G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
    if(!secondaries)
      return;

    /*
     * We create info from the parent track info and pass it to the daughter (so info for each particle is the info of the closest parent, not the original event particle)
     */
    KM3TrackInformation* anInfo = new KM3TrackInformation(aTrack);
    size_t nSeco = secondaries->size();
    if(nSeco>0)
      {
        for(size_t i=0;i<nSeco;i++)
          {
            /*
             * The global particle time will be adjusted here for decays occuring longer than 10s (1e10 ns).
             * Time is adjusted by doing lg(time)*1e5.
             * Long decays will produce particles separated in time but precision won't be lost.
             * It may happen that time arrow of the decays in the chain gets screwed (later decays will appear to be earlier). 
             * However, if the decay time of the two decays is not similar they will be appear separeted in time for the coincidences.
             * For 20 ns time window the decay time difference should be <0.05% to wrongly appear in the same time window.
             *
             */
            G4double globtime = (*secondaries)[i]->GetGlobalTime();
            if (globtime > 1e10) {
              globtime = log(globtime)/log(10)*1e5;
              (*secondaries)[i]->SetGlobalTime(globtime);
            }
            KM3TrackInformation* infoNew = new KM3TrackInformation (anInfo);
            (*secondaries)[i]->SetUserInformation(infoNew);
          }
      }
    delete (anInfo);
  }

}

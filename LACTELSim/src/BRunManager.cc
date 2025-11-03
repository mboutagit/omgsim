#include <BRunManager.hh>
#include "KM3RunManager.hh"

namespace lactel
{

  // BRunManager*  BRunManager::runManager=0;
  // BRunManager* BRunManager::GetRunManager(){
  //   if(!G4RunManager::GetRunManager()){
  //     return (BRunManager*)  (new G4RunManager());
  //   }
  //   else{
  //     return (BRunManager*) G4RunManager::GetRunManager();
  //   }
  // }

  BRunManager::BRunManager () : event_(static_cast<const BEvent*> (G4RunManager::GetRunManager()->GetCurrentEvent())){}
  BRunManager::~BRunManager (){}

  // G4Event* BRunManager::GenerateEvent(G4int eventID) {
  //     return new BEvent(eventID);
  // }

  // const BEvent*  BRunManager::GetCurrentEvent(){
  //   //return (const BEvent*) (GetCurrentEvent());
  //   return event_; 
  // }

}
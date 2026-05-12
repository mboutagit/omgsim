#include "BVolumesManager.hh"

namespace lactel
{
    BVolumesManager* BVolumesManager::Instance = 0;
    BVolumesManager::BVolumesManager() : volumes (*(KM3PassiveVolumeMgr::GetIt())){};
    BVolumesManager* BVolumesManager::GetInstance(){
        if(Instance == nullptr)
            return new BVolumesManager();
        else 
            return (BVolumesManager*) Instance;
    }

    BVolumesManager::~BVolumesManager(){};

    void BVolumesManager::getListOfVolumeNames(std::ostream& out){
        // out << "\nUser Volumes: \n";
        // if(!volumes.getListOfVolumes().size()){
        //     out << string(20, '-') << " 0 Volume defined! " << string(20, '-') << "\n\n";
        //     return;
        // }
        // out << string(20, '-') << " " << volumes.getListOfVolumes().size() << " Volume defined " << string(20, '-') << "\n";
        // for(auto& entry : volumes.getListOfVolumes())
        //     out << string(10, '-') << entry.first << std::endl;
        // out << "\n\n";
    }
} // namespace lactel

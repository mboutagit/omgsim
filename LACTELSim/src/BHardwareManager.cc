#include "BHardwareManager.hh"

namespace lactel
{
    BHardwareManager* BHardwareManager::instance = 0;
    BHardwareManager::BHardwareManager(){

    }
    BHardwareManager* BHardwareManager::getInstance(){
        if(instance)
            return (BHardwareManager*) instance;
        return new BHardwareManager();
    }

    void BHardwareManager::addOM(OM& om){
        if(oms.count(om)){
            std::cout << "\033[1;35mThe OM " << om.getOMID() << " already exists .. \033[0m" << std::endl;
        }
        oms.insert(om);
        std::cout << "\033[1;35mOM " << om.getOMID() << " Has been added. \033[0m" << std::endl;
    }

    // void BHardwareManager::addWaveCatcher(BWaveCatcher& wc){
    //     if(waveCatchers.count(wc)){
    //         std::cout << "\033[1;35mThe OM " << wc.getOMID() << " already exists .. \033[0m" << std::endl;
    //     }
    //     waveCatchers.insert(wc);
    //     std::cout << "\033[1;35mOM " << wc.getOMID() << " Has been added. \033[0m" << std::endl;
    // }

} // namespace lactel

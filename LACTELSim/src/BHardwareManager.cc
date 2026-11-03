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
            BLOG_WARNING(Form("OM %s already exists!", om.getOMID().c_str()));
        }
        oms.insert(om);
        BLOG_INFO(Form("OM %s has been added ..", om.getOMID().c_str()));
    }
} // namespace lactel

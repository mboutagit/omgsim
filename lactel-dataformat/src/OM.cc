#include "OM.hh"

namespace lactel
{
    
    std::set<std::string> OM::om_ids;
    int OM::nInstance = 0;

    OM::OM(){
        om_id="OMS0000"; // default om ref;
    }
    OM::OM(std::string id){
        if(om_ids.count(id)){
            std::cerr << "\033[1;31mError: OM with id " + id + " already defined!\033[0m" << std::endl;
            return;
        }
        om_id = id;
        om_ids.insert(id);
        id = nInstance;
        nInstance++;
    }

    OM::~OM(){}

    int OM::getOMNumber() const {
        std::string digits = om_id.substr(3, 4);
        return std::stoi(digits);
    }

    bool OM::operator<(const OM& om) const {
        return getOMNumber() < om.getOMNumber();
    }

    bool OM::operator>(const OM& om) const {
        return getOMNumber() > om.getOMNumber();
    }

    bool OM::operator==(const OM& om) const {
        return getOMNumber() == om.getOMNumber();
    }
} // namespace lactel

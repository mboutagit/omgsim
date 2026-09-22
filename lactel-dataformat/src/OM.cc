#include "OM.hh"

using namespace utils;
namespace lactel
{
    

    std::ostream& operator<<(std::ostream& os, const OM& obj){
        os << obj.om_id;
        return os;
    }

    std::set<std::string> OM::om_ids;
    std::set<OM*> OM::instances;
    int OM::nInstance = 0;
    ConnectionInfos OM::con = ConnectionInfos(string(DATABASE_CONFIGFILE));

    std::mt19937 OM::rng_ = std::mt19937(std::random_device{}());
    std::uniform_int_distribution<int> OM::noiseDist_(-2, 2);
    std::normal_distribution<> OM::TTSDist_(3.314, 0.1531*0.1531);
    std::normal_distribution<> OM::AmpDist_(0, 0.1156);

    // OM::OM(): isOK (false), om_id("OMS0000"){
    //     id = nInstance;
    //     nInstance++;
    //     om_ids.insert(om_id);
    //     instances.insert(this);
    // } // default OM ref isOK should be false

    OM::OM(std::string id) : isOK(true), om_id(id){
        if(om_ids.count(id)){
            // std::cerr << "\033[1;31mError: OM with id " + id + " already defined!\033[0m" << std::endl;
            BLogger::getInstance()->log(BLogger::Level::WARNING, "OM with id " + id + " already defined!");
            return;
        }
        om_ids.insert(id);
        id = nInstance;
        nInstance++;
        instances.insert(this);
    }

    OM::OM(){}

    OM::~OM(){
        om_ids.erase(this->om_id);
        nInstance--;
    }

    std::string OM::getOMID() const {
        return om_id;
    }

    int OM::getID() const {
        return id;
    }


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
        return getOMNumber() == om.getOMNumber() &&
                id == om.id &&
                om_id == om.om_id;
    }

    void OM::uploadOMValuesFormDB(){
        dbRoutines dbAccess(con);
        bool success = false;

        std::unique_ptr<sql::PreparedStatement> pstmt(
                        dbAccess.getConnection()->prepareStatement("select * from om_test_result where om_id=?")
        );

        pstmt->setString(1, om_id);  

         try{
                std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
                success = true;

                int size=0;
                while (res->next()) {     // <-- move cursor to next row
                    properties.hv = res->getDouble("high_voltage");
                    properties.rt = res->getDouble("rise_time");
                    properties.ft = res->getDouble("fall_time");
                    properties.wt = res->getDouble("width_time");
                    properties.g = res->getDouble("gain");
                    properties.pv = res->getDouble("peak_to_valley");
                    properties.dc = res->getDouble("dark_count");

                    size++;
                }
                if(! size){
                    BLogger::getInstance()->log(BLogger::Level::WARNING, "No data found for " + om_id + " in the database." );
                }
                else    
                    BLogger::getInstance()->log(BLogger::Level::INFO, "Data for " + om_id + " has been loaded from database." );
        }
        catch(const std::exception& e){
                std::cerr << e.what() << '\n';
            }
    }
} // namespace lactel

#ifndef dbRoutines_hh 
#define dbRoutines_hh 

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <filesystem>


using namespace std;
namespace fs = std::filesystem;

namespace lactel{

    class ConnectionInfos{

        public:

        ConnectionInfos(std::string credentials){

            if (!fs::exists(credentials)) {
                throw std::runtime_error("Configuration file not found: " + credentials);
            }
        
            if (!fs::is_regular_file(credentials)) {
                throw std::runtime_error("Configuration path is not a file: " + credentials);
            }
        
            // Verify file permissions (optional but recommended for security)
            auto perms = fs::status(credentials).permissions();
            if ((perms & fs::perms::others_read) != fs::perms::none) {
                throw std::runtime_error("Insecure file permissions on config file - should be 600");
            }
        
            try {
                boost::property_tree::ini_parser::read_ini(credentials, pt);
            } catch (const boost::property_tree::ini_parser_error& e) {
                throw std::runtime_error("Failed to parse config file: " + std::string(e.what()));
            }
        
            // Validate required fields exist
            const std::vector<std::string> requiredFields = {
                "omtest_db.host",
                "omtest_db.user",
                "omtest_db.password",
                "omtest_db.database"
            };
        
            for (const auto& field : requiredFields) {
                if (!pt.get_optional<std::string>(field)) {
                    throw std::runtime_error("Missing required field in config: " + field);
                }
            }

            boost::property_tree::ini_parser::read_ini(credentials, pt);

            hostname = pt.get<std::string>("omtest_db.host");
            username = pt.get<std::string>("omtest_db.user");
            password = pt.get<std::string>("omtest_db.password");
            database = pt.get<std::string>("omtest_db.database");

        }

        ConnectionInfos(std::string hn, std::string un, std::string pw){
            hostname = hn;
            username = un;
            password = pw;
        }
        void setHost(std::string host) {hostname = host;};
        void setUserName(std::string un) {username = un;};
        void setUserPassword(std::string pw) {password = pw;};

        std::string getHost() {return hostname;};
        std::string getUserName() {return username;};
        std::string getUserPassword() {return password;};
        std::string getDatabase() {return database;};

        private:
            std::string hostname;
            std::string username;
            std::string password;
            std::string database;
            boost::property_tree::ptree pt;


        friend class dbRoutines;
    };

    class dbRoutines{

        public:

            dbRoutines(ConnectionInfos& con) : driver(get_driver_instance()), connection(driver->connect(con.hostname, con.username, con.password)){
                connection->setSchema(con.database);
                cout << "\r\033[10C" << "Connection to " << con.database << " database OK!" << endl;
            };
           
            sql::Driver* getDriver(){return driver;};
            std::unique_ptr<sql::Connection>& getConnection(){return connection;};

        private:
            sql::Driver* driver;
            std::unique_ptr<sql::Connection> connection;
    };
}

#endif
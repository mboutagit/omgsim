#ifndef BLOGGER_hh
#define BLOGGER_hh

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <sstream>

using namespace std;

namespace utils // LACTEL
{

    /**
     * @brief class for logging
     * @author M. Bouta bouta@cppm.in2p3.fr
     */
    
    class BLogger{
        private:
            BLogger();
            static BLogger* instance; 
            std::string filename;
            std::string filePath;   
        public:

            enum class Level {DEBUG, INFO, WARNING, ERROR};

            ~BLogger(){};
            static BLogger* getInstance();

            void setOutputFile(const std::string& filename);
            void setOutputFile(std::ofstream&& file);
            std::string levelToString(Level lvl);
            void log(Level level, const std::string& msg, const char* function, int line);

            int debugLevel = 0;
            std::ostream* out = &std::cout;

            void setDebugLevel(int dl){debugLevel = dl;}
            int getDebugLevel() {return debugLevel;}
            void setOutput(std::ostream& output){out = &output;}
            void printOutput() const;
            static std::string getCurrentTimestampForFileNaming();
            static const Level getLevel(); 

            std::string timeStamp();
            bool writeToFile_;
            std::ofstream file_;
            std::mutex mutex_;

    };    
} // namespace utils

#define BLOG_DEBUG(msg) \
    utils::BLogger::getInstance()->log(utils::BLogger::Level::DEBUG, msg, __func__, __LINE__)

#define BLOG_INFO(msg) \
    utils::BLogger::getInstance()->log(utils::BLogger::Level::INFO, msg, __func__, __LINE__)

#define BLOG_WARNING(msg) \
    utils::BLogger::getInstance()->log(utils::BLogger::Level::WARNING, msg, __func__, __LINE__)

#define BLOG_ERROR(msg) \
    utils::BLogger::getInstance()->log(utils::BLogger::Level::ERROR, msg, __func__, __LINE__)

#endif
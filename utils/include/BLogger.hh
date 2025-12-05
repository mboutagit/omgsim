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

namespace utils
{
    class BLogger{
        private:
            BLogger();
            static BLogger* instance;    
        public:

            enum class Level {
                    DEBUG,
                    INFO,
                    WARNING,
                    ERROR
            };

            ~BLogger();
            static BLogger* getInstance();

            void setOutputFile(const std::string& filename);
            std::string levelToString(Level lvl);
            void log(Level level, const std::string& msg);
                // Convenience macros
            void info(const std::string& msg)    { log(Level::INFO, "\033[1;34m" + msg + "\033[0m"); }
            void warn(const std::string& msg)    { log(Level::WARNING, ("Wa33" + msg + "\033[0m").c_str());}
            void error(const std::string& msg)   { log(Level::ERROR, "\033[1;34m" + msg + "\033[0m"); }
            void debug(const std::string& msg)   { log(Level::DEBUG, "\033[1;34m" + msg + "\033[0m"); }

            std::string timeStamp();

            bool writeToFile_;
            std::ofstream file_;
            std::mutex mutex_;
    };    
} // namespace utils

#endif
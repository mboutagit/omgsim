#include "BLogger.hh"

#include <iomanip>
#include <ctime>
#include <sstream>

namespace utils
{
    BLogger* BLogger::instance = 0;

    BLogger::BLogger(){};
    BLogger* BLogger::getInstance(){
        if(!instance)
            return new BLogger();
        return (BLogger*)  instance;
    }

    void BLogger::setOutputFile(const std::string& filename) {
        // std::lock_guard<std::mutex> lock(mutex_);
        file_.open(filename, std::ios::app);
        writeToFile_ = file_.is_open();
    }

    // The main logging function
    void BLogger::log(Level level, const std::string& msg) {
       // std::lock_guard<std::mutex> lock(mutex_);

        std::string levelStr = levelToString(level);
        std::string timestamp = timeStamp();

        std::string formatted = "[" + timestamp + "][" + levelStr + "] " + msg;

        // Always print to console
        std::cout << formatted << std::endl;

        // Optionally write to file
        if (writeToFile_) {
            file_ << formatted << std::endl;
        }
    }
    std::string BLogger::levelToString(Level lvl) {
        switch(lvl) {
            case Level::DEBUG:   return "DEBUG";
            case Level::INFO:    return "INFO";
            case Level::WARNING: return "WARN";
            case Level::ERROR:   return "ERROR";
        }
        return "UNKNOWN";
    }

    std::string BLogger::timeStamp() {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
} // namespace utils

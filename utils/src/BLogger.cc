#include "BLogger.hh"

#include <iomanip>
#include <ctime>
#include <sstream>

namespace utils // LACTEL
{
    BLogger* BLogger::instance = 0;

    BLogger::BLogger(){};
    BLogger* BLogger::getInstance(){
        if(!instance)
            instance = new BLogger();
        return (BLogger*) instance;
    }

    /**
     * @brief function to set the output file name as string
     * @param filename
     */
    void BLogger::setOutputFile(const std::string& filename) {
        file_.open(filename, std::ios::out | std::ios::app);

        if (!file_.is_open()) {
            out = &std::cout;
            writeToFile_ = false;
            throw std::runtime_error("Cannot open log file: " + filename);
        }

        writeToFile_ = true;
        out = &file_;
    }

    /**
     * @brief function to set the output file as ofstream
     * @param file 
     */
   void BLogger::setOutputFile(std::ofstream&& file) {
        file_ = std::move(file);
        writeToFile_ = file_.is_open();

        out = writeToFile_ ? static_cast<std::ostream*>(&file_)
                        : static_cast<std::ostream*>(&std::cout);
    }

    void BLogger::printOutput() const {
        if (out == &std::cout) {
            std::cout << "Output: std::cout" << std::endl;
        } else if (out == &file_) {
            std::cout << "Output: file_" << std::endl;
        } else {
            std::cout << "Output: unknown stream at " << out << std::endl;
        }
    }


    // The main logging function
    /**
     * @brief function for logging
     * @param level, the debug level 
     * @param msg, the message to show
     * @param function, the function from where the log is issued
     * @param line, the line of from which the log message is issued
     */
    void BLogger::log(Level level, const std::string& msg, const char* function, int line) {

        std::string levelStr = levelToString(level);
        std::string timestamp = timeStamp();

         std::string formatted =
        "[" + std::string(function) + ":" + std::to_string(line) + "]"
        + "[" + timestamp + "]"
        + "[" + levelStr + "] "
        + msg;

        if (debugLevel >= 4 ||
            (debugLevel >= 3 && level != Level::DEBUG) ||
            (debugLevel >= 2 && level != Level::DEBUG && level != Level::INFO) ||
            (debugLevel >= 1 && level == Level::ERROR))
        {
            *out << formatted << std::endl;
        }
    }

    /**
     * @brief helper function to retrun debug level as tring
     * @param lvl, Level
     */
    std::string BLogger::levelToString(Level lvl) {
        switch(lvl) {
            case Level::DEBUG:   return "DEBUG";
            case Level::INFO:    return "INFO";
            case Level::WARNING: return "WARN";
            case Level::ERROR:   return "ERROR";
        }
        return "UNKNOWNLEVEL";
    }

    /**
     * @brief function to return the current timestamp as string
     */
    std::string BLogger::timeStamp() {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    /**
     * @brief function to get formatted timestamp for files naming as string
     */
    std::string BLogger::getCurrentTimestampForFileNaming()
    {
        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);

        std::tm localTime{};
    #ifdef _WIN32
        localtime_s(&localTime, &time);
    #else
        localtime_r(&time, &localTime);
    #endif

        std::ostringstream oss;
        oss << std::put_time(&localTime, "%d-%m-%y.%H-%M");

        return oss.str();
    }
} // namespace utils

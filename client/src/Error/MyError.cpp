#include "MyError.hpp"

using namespace client;

static std::tm *getCurrentTime() {
    std::chrono::system_clock::time_point date = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(date);
    return std::localtime(&time);
}

void client::log(const std::string &message, const std::string &type) {
    // std::tm *currentTime = getCurrentTime();

    // std::stringstream dateStream;
    // dateStream << std::put_time(currentTime, "%Y-%m-%d");
    // std::string dateStr = dateStream.str();

    // std::filesystem::create_directory("./logs");

    // std::string fileName = "./logs/" + dateStr + ".log";
    // std::ofstream file(fileName, std::ios::app);

    // if (file.is_open()) {
    //     file << std::put_time(currentTime, "%H:%M:%S") << " [" << type << "] " << message << std::endl;
    //     file.close();
    // }
}

MyError::MyError(const std::string &className, const std::string &message, bool logToFile, const std::string &errorType): _msg(className + ": " + message) {
    if (logToFile)
        log(_msg, errorType);
}

const char *MyError::what() const noexcept {
    return _msg.c_str();
}

#include "MyError.hpp"

static std::tm *getCurrentTime() {
    std::chrono::system_clock::time_point date = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(date);
    return std::localtime(&time);
}

void log(const std::string &message, const std::string &type) {
    std::filesystem::create_directory("./logs");
    std::ofstream myfile("./logs/error.log", std::ios::app);
    if (myfile.is_open()) {
        myfile << std::put_time(getCurrentTime(), "%Y-%m-%d %H:%M:%S") << " [" << type << "] " << message << std::endl;
        myfile.close();
    }
}

MyError::MyError(const std::string &className, const std::string &message, bool logToFile, const std::string &errorType): _msg(className + ": " + message) {
    if (logToFile)
        log(_msg, errorType);
}

const char *MyError::what() const noexcept {
    return _msg.c_str();
}

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>

namespace client {
    void log(const std::string &message, const std::string &type = "LOG");

    class MyError: public std::exception {
        public:
            MyError(const std::string &className, const std::string &message, bool logToFile = true, const std::string &errorType = "ERROR");
            virtual const char *what() const noexcept override;

        protected:
            std::string _msg;
    };
}

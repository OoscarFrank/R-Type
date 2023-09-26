#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "MyError.hpp"

/**
 * @brief Class that can be used to parse command line arguments and get flags values
 *
 */
class Args {
    public:
        class Error: public MyError {
            public:
                Error(const std::string &message, const std::string &className = "Args"): MyError(className, message, true, "ERROR") {}
        };
        class Warning: public MyError {
            public:
                Warning(const std::string &message, const std::string &className = "Args"): MyError(className, message, true, "WARN") {}
        };
        class Info: public MyError {
            public:
                Info(const std::string &message, const std::string &className = "Args"): MyError(className, message, false, "INFO") {}
        };

        /**
         * @brief Construct a new Args object and parse the command line arguments
         *
         * @param ac Number of arguments
         * @param av Arguments
         */
        Args(int ac, char **av);
        Args(const Args &other) = delete;
        Args(Args &&other) = delete;
        ~Args() = default;

        Args &operator=(const Args &other) = delete;
        Args &operator=(Args &&other) = delete;

        /**
         * @brief Check if a flag is entered in the command line arguments
         *
         * @param flag Flag to check
         * @return true If the flag is set
         * @return false If the flag is not set
         */
        bool isFlagSet(const std::string &flag) const;

        /**
         * @brief Get the value of the flag
         *
         * @tparam T Type of the value
         * @param flag Flag to get the value of
         * @return T
         */
        template<typename T>
        T getFlagValue(const std::string &flag) const;
        /**
         * @brief Get the value of the flag or a default value if the flag is not set
         *
         * @tparam T Type of the value
         * @param flag Flag to get the value of
         * @param defaultValue Default value if the flag is not set
         * @return T
         */
        template<typename T>
        T getFlagValue(const std::string &flag, const T &defaultValue) const;
        /**
         * @brief Get the values of the flag
         *
         * @tparam T Type of the values
         * @param flag Flag to get the values of
         * @return std::vector<T>
         */
        template<typename T>
        std::vector<T> getFlagValues(const std::string &flag) const;
        /**
         * @brief Get the values of the flag or default values if the flag is not set
         *
         * @tparam T Type of the values
         * @param flag Flag to get the values of
         * @param defaultValues Default values if the flag is not set
         * @return std::vector<T>
         */
        template<typename T>
        std::vector<T> getFlagValues(const std::string &flag, const std::vector<T> &defaultValues) const;

    protected:
    private:
        std::vector<std::string> _tokens;
};

template<typename T>
T Args::getFlagValue(const std::string &flag) const {
    auto it = std::find(_tokens.begin(), _tokens.end(), flag);
    if (it == _tokens.end())
        throw Error("Flag " + flag + " not found");
    if (++it == _tokens.end() || (*it).at(0) == '-')
        throw Error("Flag " + flag + " has no value");

    std::stringstream ss(*it);
    T res;
    if (!(ss >> res))
        throw Warning("Invalid conversion");
    return res;
}

template<typename T>
T Args::getFlagValue(const std::string &flag, const T &defaultValue) const {
    auto it = std::find(_tokens.begin(), _tokens.end(), flag);
    if (it == _tokens.end())
        return defaultValue;
    if (++it == _tokens.end() || (*it).at(0) == '-')
        return defaultValue;

    std::stringstream ss(*it);
    T res;
    if (!(ss >> res))
        return defaultValue;
    return res;
}

template<typename T>
std::vector<T> Args::getFlagValues(const std::string &flag) const {
    std::vector<T> res;
    auto it = std::find(_tokens.begin(), _tokens.end(), flag);
    if (it == _tokens.end())
        return res;
    while (++it != _tokens.end() && (*it).at(0) != '-') {
        std::stringstream ss(*it);
        T tmp;
        if (!(ss >> tmp))
            throw Warning("Invalid conversion");
        res.push_back(tmp);
    }
    return res;
}

template<typename T>
std::vector<T> Args::getFlagValues(const std::string &flag, const std::vector<T> &defaultValues) const {
    std::vector<T> res;
    auto it = std::find(_tokens.begin(), _tokens.end(), flag);
    if (it == _tokens.end())
        return defaultValues;
    while (++it != _tokens.end() && (*it).at(0) != '-') {
        std::stringstream ss(*it);
        T tmp;
        if (!(ss >> tmp))
            throw Warning("Invalid conversion");
        res.push_back(tmp);
    }
    return res;
}

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "MyError.hpp"

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

        Args(int ac, char **av);
        Args(const Args &other) = delete;
        Args(Args &&other) = delete;
        ~Args();

        Args &operator=(const Args &other) = delete;
        Args &operator=(Args &&other) = delete;

        bool isFlagSet(const std::string &flag) const;

        template<typename T>
        T getFlagValue(const std::string &flag) const;
        template<typename T>
        T getFlagValue(const std::string &flag, const T &defaultValue) const;
        template<typename T>
        std::vector<T> getFlagValues(const std::string &flag) const;
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

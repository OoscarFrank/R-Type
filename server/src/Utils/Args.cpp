#include "Args.hpp"

Args::Args(int ac, char **av) {
    for (int i = 0; i < ac; ++i)
        _tokens.push_back(std::string(av[i]));
}

bool Args::isFlagSet(const std::string &flag) const {
    return std::find(_tokens.begin(), _tokens.end(), flag) != _tokens.end();
}

// #include <signal.h>
#include "Core.hpp"

using namespace client;


int main(int ac, char **av)
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    Core core;
    try {
        if (core.checkArgs(ac, av) == 0) {
            core.run();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}

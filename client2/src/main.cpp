// #include <signal.h>
#include "Core.hpp"
#include "Network/Network.hpp"

using namespace client;

int main(int ac, char **av)
{
    // signal(SIGPIPE, SIG_IGN);
    Core core;
    try {
        if (core.checkArgs(ac, av) == 0) {
            Network net;
            net.setInst(9);
            net.send();
            core.run();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}

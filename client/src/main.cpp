#include <signal.h>
#include "Core.hpp"
// #include "Game.hpp"
using namespace Client;

int main(int ac, char **av)
{
    signal(SIGPIPE, SIG_IGN);
    Core core;
    try
    {
        core.checkArgs(ac, av);
        core.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
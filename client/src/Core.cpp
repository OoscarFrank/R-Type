#include "Core.hpp"

using namespace client;
using namespace game;

Core::Core()
{
    this->_port = 4242;
    this->_ip = "127.0.0.1";
}

int Core::checkArgs(int ac, char **av)
{
    if (ac == 1)
        throw MyError("Core", "Not enough arguments.");
    if (ac > 5)
        throw MyError("Core", "Too many arguments.");
    if ((ac == 2) && (std::string(av[1]).find("-help") != std::string::npos)) {
        std::cout << "USAGE: ./client -p port -h machine\n\tport\tis the port number\n\tmachine\tis the name of the machine; localhost by default" << std::endl;
        return -1;
    }

    // for (int i = 0; i < ac; i++) {
    //     if (std::string(av[i]).find("-p") != std::string::npos) {
    //         if (av[i + 1] == NULL)
    //             throw MyError("Core", "You put the -p but no port after.");
    //         try {
    //             this->_port = std::stoi(av[i + 1]);
    //         } catch (std::exception &e) {
    //             throw MyError("Core", "Port is not a number.");
    //         }
    //     }
    //     if (std::string(av[i]).find("-h") != std::string::npos) {
    //         if (av[i + 1] == NULL)
    //             throw MyError("Core", "You put the -h but no IP adress after.");
    //         for (int j = 0; av[i + 1][j] != '\0'; j++) {
    //             if (av[i + 1][j] == '.')
    //                 continue;
    //             else if (av[i + 1][j] < '0' || av[i + 1][j] > '9')
    //                 throw MyError("Core", "IP adress is not a number.");
    //             else
    //                 this->_ip = av[i + 1];
    //         }
    //     }
    // }
    return 0;
}

void Core::run()
{
    Game game;
    game.MainLoop();
}

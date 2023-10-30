#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Error/MyError.hpp"
#include "Game/Game.hpp"

namespace client {
   class Core {
        public:
            Core();
            ~Core(){};

            int checkArgs(int ac, char *av[]);
            void run();

        private:
            int _port;
            std::string _ip;
    };
}

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Utils/MyError.hpp"
// #include "Game.hpp"

namespace Client {
   class Core {
        public:
            Core();
            ~Core(){};
            void checkArgs(int ac, char **av);
            void run();
        protected:
            int _port;
            std::string _ip;
        private:
    };
}

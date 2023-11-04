#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Error/MyError.hpp"
#include "Game/Game.hpp"

namespace client {
   class Core {
        public:
        /**
         * @brief Construct a new Core object
         * 
         */
            Core();
        /**
         * @brief Destroy the Core object
         * 
         */
            ~Core(){};
        /**
         * @brief Function to check the arguments
         * 
         * @param ac 
         * @param av 
         * @return int 
         */
            int checkArgs(int ac, char *av[]);
        /**
         * @brief Run the program
         * 
         */
            void run();
        private:
        /**
         * @brief Store the port
         * 
         */
            int _port;
        /**
         * @brief Store the ip
         * 
         */
            std::string _ip;
    };
}

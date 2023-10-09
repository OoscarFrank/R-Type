#pragma once

#include <iostream>
#include <filesystem>
#include "Error/MyError.hpp"

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

namespace client {
    std::string getAssetPath(std::string path);
}

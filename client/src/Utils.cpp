#include "Utils.hpp"

std::string client::getAssetPath(std::string path)
{
    if (path[0] != '/')
        path = "/" + path;
    std::string localPath = "./assets" + path;
    std::string globalPath = "/usr/share/r-type_client/assets" + path;

    if (std::filesystem::exists(localPath))
        return localPath;
    else if (std::filesystem::exists(globalPath))
        return globalPath;
    throw client::MyError("getAssetPath", "Asset not found: " + path);
}

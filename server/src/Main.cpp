#include <iostream>

#include "Reader/Reader.hpp"


int main(int argc, char **argv)
{

    Reader reader(4242);
    std::vector<std::unique_ptr<Client>> clients;

    reader.Clock(clients);
    return 0;
}

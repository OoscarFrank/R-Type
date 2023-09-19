#pragma once

#include <iostream>
#include <vector>

#include "Utils/Queue.hpp"
#include <asio.hpp>

class Client
{
private:
    asio::ip::udp::endpoint _endpoint;
    std::string buffer;

public:
    Client(asio::ip::udp::endpoint endpoint);
    ~Client();
    const asio::ip::udp::endpoint &getEndpoint() const;
    void pushBuffer(const std::string &data);
    std::pair<size_t, std::string> getNextInst();
};



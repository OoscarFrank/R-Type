#pragma once

#include <iostream>
#include <vector>

#include "Utils/Queue.hpp"
#include <asio.hpp>

class Client
{
private:
    asio::ip::udp::endpoint _endpoint;
    
public:
    Client(asio::ip::udp::endpoint endpoint);
    ~Client();
    const asio::ip::udp::endpoint &GetEndpoint() const;
};



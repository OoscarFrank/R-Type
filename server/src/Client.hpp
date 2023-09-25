#pragma once

#include <iostream>
#include <vector>

#include "Utils/Queue.hpp"
#include <asio.hpp>

class Client
{
private:
    asio::ip::udp::socket &_socket;
    asio::ip::udp::endpoint _endpoint;
    std::string buffer;
    std::string _dataOut;
    unsigned char _instOut;
    unsigned short _roomId;
    unsigned int lastActivity;

public:
    Client(asio::ip::udp::socket &socket, asio::ip::udp::endpoint endpoint);
    ~Client();
    Client(const Client &client) = delete;
    Client(Client &&client) = delete;
    Client &operator=(const Client &client) = delete;
    Client &operator=(Client &&client) = delete;
    const asio::ip::udp::endpoint &getEndpoint() const;
    void pushBuffer(const std::string &data);
    std::pair<size_t, std::string> getNextInst();
    void catCharOut(const char &data);
    void catShortOut(const short &data);
    void catIntOut(const int &data);
    void setInst(unsigned char inst);
    void send();
    void send(const std::string &message);
};

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

#include  "../Utils/Packet.hpp"

#include "../Client.hpp"
#include "../Utils/Queue.hpp"

class Reader
{
private:
    std::thread _thread;
    asio::io_context _ioContext;
    asio::ip::udp::socket &_socket;
    Queue<Packet> &_queueIn;
    void Clock();

public:
    Reader(asio::ip::udp::socket &socket, Queue<Packet> &queueIn);
    ~Reader();

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };
};

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

#include "../Client.hpp"

class Reader
{
private:
    std::thread _thread;
    asio::io_context _ioContext;
    asio::ip::udp::socket &_socket;
    std::vector<std::unique_ptr<Client>> &_clients;
    void Clock();

public:
    Reader(asio::ip::udp::socket &socket, std::vector<std::unique_ptr<Client>> &clients);
    ~Reader();

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };
};

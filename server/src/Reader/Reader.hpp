#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

#include "../Client.hpp"

class Reader
{
private:
    asio::io_context _ioContext;
    asio::ip::udp::socket _socket;

public:
    Reader(int port = 4242);
    ~Reader();
    void Clock(std::vector<std::unique_ptr<Client>> &clients);

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };
};

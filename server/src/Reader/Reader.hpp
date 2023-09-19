#pragma once

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>


#include <set>
#include <vector>
#include <memory>
#include "../Client.hpp"
#include <utility>


class Reader
{
private:
    asio::io_context _ioContext;
    asio::ip::udp::socket _socket;

public:
    Reader(int port = 4242);
    ~Reader();
    void Clock(std::vector<std::unique_ptr<Client>> &clients);

    // class SocketError : public std::exception
    // {
    // public:
    //     virtual const char *what() const noexcept { return "[ERROR] Socket error"; }
    // };

    // class SocketOptError : public std::exception
    // {
    // public:
    //     virtual const char *what() const noexcept { return "[ERROR] Socket Options error"; }
    // };

    // class BindError : public std::exception
    // {
    // public:
    //     virtual const char *what() const noexcept { return "[ERROR] Bind error"; }
    // };
    
    // class ListenError : public std::exception
    // {
    // public:
    //     virtual const char *what() const noexcept { return "[ERROR] Listen error"; }
    // };

    // class SelectError : public std::exception
    // {
    // public:
    //     virtual const char *what() const noexcept { return "[ERROR] Select error"; }
    // };
};

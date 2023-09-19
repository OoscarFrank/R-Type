#pragma once

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <thread>
#include <set>
#include <vector>
#include <memory>
#include "../Client.hpp"
#include <utility>


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

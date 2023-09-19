#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

#include "../Utils/Packet.hpp"

#include "../Client.hpp"
#include "../Utils/Queue.hpp"

class Reader
{

public:

    class Packet
    {
    private:
        Client &_client;
        int _instruction;
        std::string _data;

    public:
        Packet(Client &client, const std::string &data, int instruction);
        ~Packet();
        int getInstruction() const;
        const std::string &getData() const;
        const Client &getClient() const;
    };

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };

    Reader(asio::ip::udp::socket &socket, Queue<std::unique_ptr<Reader::Packet>> &queueIn, std::vector<std::unique_ptr<Client>> &clients);
    ~Reader();

private:
    std::thread _thread;
    asio::io_context _ioContext;
    asio::ip::udp::socket &_socket;
    Queue<std::unique_ptr<Reader::Packet>> &_queueIn;
    std::vector<std::unique_ptr<Client>> &_clients;
    void Clock();
};

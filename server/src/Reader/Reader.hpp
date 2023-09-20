#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include "../Client.hpp"
#include "../Utils/Queue.hpp"

class Reader
{

public:

    class Packet
    {
    private:
        Client &_client;
        std::string _data;
        int _instruction;

    public:
        Packet(Client &client, const std::string &data, int instruction);
        ~Packet();
        int getInstruction() const;
        const std::string &getData() const;
        Client &getClient() const;
    };

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };

    Reader(asio::ip::udp::socket &socket, Queue<Reader::Packet> &queueIn, std::vector<Client> &clients);
    ~Reader();

private:
    std::thread _thread;
    asio::io_context _ioContext;
    asio::ip::udp::socket &_socket;
    Queue<Reader::Packet> &_queueIn;
    std::vector<Client> &_clients;
    void Clock();
};

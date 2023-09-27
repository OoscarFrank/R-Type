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
        std::shared_ptr<Client> _client;
        std::string _data;
        int _instruction;

    public:
        Packet(std::shared_ptr<Client> client, const std::string &data, int instruction);
        ~Packet();
        int getInstruction() const;
        const std::string &getData() const;
        std::shared_ptr<Client> getClient() const;
        int getDataInt();
        short getDataShort();
        char getDataChar();

    };

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };

    Reader(asio::ip::udp::socket &socket, Queue<Reader::Packet> &queueIn, std::vector<std::shared_ptr<Client>> &clients);
    ~Reader();

private:
    std::thread _thread;
    asio::io_context _ioContext;
    asio::ip::udp::socket &_socket;
    Queue<Reader::Packet> &_queueIn;
    std::vector<std::shared_ptr<Client>> &_clients;
    void Clock();
};

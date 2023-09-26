#pragma once

#include <iostream>
#include <asio.hpp>
#include "Queue.hpp"
#include "Instruction.hpp"

class Network
{
public:
    class Packet
    {
    private:
        std::string _data;
        int _instruction;

    public:
        Packet(const std::string &data, int instruction);
        ~Packet();
        int getInstruction() const;
        const std::string &getData() const;
        int getDataInt();
        short getDataShort();
        char getDataChar();
    };

    Network(std::string ip = "127.0.0.1", int port = 4242);
    ~Network();
    void catCharOut(const char &data);
    void catShortOut(const short &data);
    void catIntOut(const int &data);
    void setInst(unsigned char inst);
    void send(const std::string &message);
    void send();
    void read();
    std::pair<size_t, std::string> getNextInst();

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };


private:
    std::string _ip;
    int _port;
    asio::io_context _ioContext;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _serverEndpoint;

    std::string _dataOut;
    unsigned char _instOut;

    std::thread _ReaderThread;
    std::string _buferIn;
    Queue<Network::Packet> _queueIn;
};
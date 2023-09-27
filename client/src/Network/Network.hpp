#pragma once

#include <iostream>
#include <asio.hpp>
#include "Queue.hpp"
#include "Instruction.hpp"
#include "Stream.hpp"

class Network
{
public:
    class Packet
    {
    private:
        Stream _data;
        int _instruction;

    public:
        Packet(const Stream &data, int instruction);
        ~Packet();
        int getInstruction() const;
        Stream &getData();
    };

    Network(std::string ip = "127.0.0.1", int port = 4242);
    ~Network();
    void setInst(unsigned char inst);
    void send(const Stream &stream);
    void send();
    void read();
    Stream &getStreamOut();
    Queue<Network::Packet> &getQueueIn();
    std::pair<size_t, Stream> getNextInst();

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

    Stream _streamOut;
    unsigned char _instOut;

    std::thread _ReaderThread;
    Stream _streamIn;
    Queue<Network::Packet> _queueIn;
};
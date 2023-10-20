#pragma once

#include <iostream>
#include <map>
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
        Packet();
        ~Packet();
        int getInstruction() const;
        Stream &getData();
    };

    Network(std::string ip = "127.0.0.1", int port = 4242);
    ~Network();
    void setInst(unsigned char inst);
    void send(const Stream &stream);
    void send();
    void resend(u_short cmdNbr);
    void read();
    Stream &getStreamOut();
    Queue<Network::Packet> &getQueueIn();
    std::pair<size_t, Stream> getNextInst();
    void startReceive();
    void setClosed(bool closed);

    class ReadError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };
    std::thread _ReaderThread;


private:
    std::string _ip;
    int _port;
    asio::io_context _ioContext;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _serverEndpoint;
    bool _closed;


    Stream _streamOut;
    unsigned char _instOut;

    Stream _streamIn;
    Queue<Network::Packet> _queueIn;

    const std::vector<Commands> _inCommands;
    const std::vector<Commands> _outCommands;
    u_short _cmdNbr;
    u_short _lastCmdNbrRecieved;
    std::map<u_short, Stream> _sentPackets;
};

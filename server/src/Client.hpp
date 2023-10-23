#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <asio.hpp>

#include "Utils/Instruction.hpp"
#include "Utils/Scheduling.hpp"
#include "Utils/Queue.hpp"
#include "Utils/Stream.hpp"

class Client
{
private:
    asio::ip::udp::socket &_socket;
    asio::ip::udp::endpoint _endpoint;
    Stream _streamIn;
    std::mutex _resendMutex;

    size_t _lastActivity;
    std::chrono::system_clock::time_point _lastPing = std::chrono::system_clock::now();
    u_short _cmdNbr;
    u_short _lastCmdNbrRecieved;
    const std::vector<Commands> _outCommands;
    const std::vector<Commands> _inCommands;
    std::map<u_short, Stream> _sentPackets;

public:
    Client(asio::ip::udp::socket &socket, asio::ip::udp::endpoint endpoint);
    ~Client();
    Client(const Client &client) = delete;
    Client(Client &&client) = delete;
    Client &operator=(const Client &client) = delete;
    Client &operator=(Client &&client) = delete;
    const asio::ip::udp::endpoint &getEndpoint() const;
    Stream &getStreamIn();
    std::pair<size_t, Stream> getNextInst();
    void send(const Stream &message);
    void resend(u_short cmdNbr);
    bool isAlive();
    void ping();
};

#endif

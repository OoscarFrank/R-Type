#pragma once

#include <iostream>
#include <vector>

#include "Utils/Queue.hpp"
#include <asio.hpp>

class Client
{
private:
    asio::ip::udp::endpoint _endpoint;
    std::string buffer;
    std::string _dataOut;
    unsigned char _instOut;
    unsigned short _roomId;
    unsigned char _gamePlayerId;
    unsigned int lastActivity;

public:
    Client(asio::ip::udp::endpoint endpoint);
    ~Client();
    const asio::ip::udp::endpoint &getEndpoint() const;
    void pushBuffer(const std::string &data);
    std::pair<size_t, std::string> getNextInst();
    void catCharOut(const char &data);
    void catShortOut(const short &data);
    void catIntOut(const int &data);
    void setInst(unsigned char inst);
    void setGamePlayerId(unsigned char id);
    unsigned char getGamePlayerId() const;
    void setRoomId(unsigned int roomId);
    unsigned int getRoomId() const;
    std::string getOutReq();
};

#include "Client.hpp"
#include "Utils/Instruction.hpp"
#include "Utils/Scheduling.hpp"
#include <bitset>

Client::Client(asio::ip::udp::socket &socket, asio::ip::udp::endpoint endpoint): _socket(socket)
{
    this->_endpoint = endpoint;
    this->_instOut = 0;
    this->_lastActivity = NOW;
}

Client::~Client()
{
}

const asio::ip::udp::endpoint &Client::getEndpoint() const
{
    return this->_endpoint;
}

Stream &Client::getStreamIn()
{
    return this->_streamIn;
}

std::pair<size_t, Stream> Client::getNextInst()
{
    if (this->_streamIn.size() == 0)
        return std::make_pair(0, Stream());
    std::vector<Commands> inst = IN_COMMANDS;
    std::pair<size_t, Stream> out;

    for (auto i = inst.begin(); i != inst.end(); ++i) {
        if (this->_streamIn[0] == i->_inst) {
            if (this->_streamIn.size() < i->_size + 1)
                return std::make_pair(0, Stream());
            out.first = i->_inst;
            out.second = this->_streamIn.subStream(1, i->_size);
            this->_streamIn = this->_streamIn.subStream(i->_size + 1);
            return out;
        }
    }
    this->_streamIn.clear();
    return std::make_pair(0, Stream());
}

void Client::setInst(unsigned char inst)
{
    _instOut = inst;
}

void Client::send(const Stream &stream)
{
    if (_endpoint.protocol() == asio::ip::udp::v4())
        _socket.send_to(asio::buffer(stream.toString()), _endpoint);
    else
        std::cerr << "Endpoint is not IPv4" << std::endl;
}

void Client::send()
{
    Stream out;
    out += _instOut;
    out += _streamOut;
    send(out);
    _streamOut.clear();
    _instOut = 0;
}

bool Client::isAlive()
{
    if (std::chrono::system_clock::now() - _lastPing > std::chrono::seconds(PLAYER_ALIVE_MAX_TIME))
        return false;
    return true;
}

void Client::ping()
{
    _lastPing = std::chrono::system_clock::now();
}

Stream &Client::getStreamOut()
{
    return this->_streamOut;
}
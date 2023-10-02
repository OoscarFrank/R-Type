#include "Network.hpp"
#include <bitset>

Network::Network(std::string ip, int port) : _socket(_ioContext, asio::ip::udp::v4()), _serverEndpoint(asio::ip::address::from_string(ip), port)
{
    this->_ReaderThread = std::thread(&Network::read, this);
    _closed = false;
}

Network::~Network()
{
    if (_ReaderThread.joinable())
        _ReaderThread.join();
}

void Network::setClosed(bool closed)
{
    _closed = closed;
}

void Network::setInst(unsigned char inst)
{
    _instOut = inst;
}

void Network::send(const Stream &stream)
{
    if (_serverEndpoint.protocol() == asio::ip::udp::v4())
        _socket.send_to(asio::buffer(stream.toString()), _serverEndpoint);
    else
        std::cerr << "Endpoint is not IPv4" << std::endl;
}

void Network::send()
{
    Stream out;
    out += _instOut;
    out += _streamOut;
    send(out);
    _streamOut.clear();
    _instOut = 0;
}

void Network::read()
{
    startReceive();
    _ioContext.run();
}

void Network::startReceive()
{
    auto data = std::make_shared<std::array<char, 1024>>();

    
    if (_closed) {
        return;
    }
    _socket.async_receive_from(
        asio::buffer(*data), _serverEndpoint,
        [this, data](const asio::error_code& ec, std::size_t bytes_transferred) {
            std::pair<size_t, Stream> tmpInst;
            if (!ec && bytes_transferred > 0) {
                this->_streamIn.setDataCharArray(data->data(), bytes_transferred);
                while ((tmpInst = this->getNextInst()).first != 0)
                    _queueIn.push(Network::Packet(tmpInst.second, tmpInst.first));
                startReceive();
            } else if (ec) {
                std::cerr << "Error inr Network" << std::endl;
            }
        }
    );
}

std::pair<size_t, Stream> Network::getNextInst()
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

Stream &Network::getStreamOut()
{
    return _streamOut;
}

Network::Packet::Packet()
{
}


Network::Packet::Packet(const Stream &data, int instruction) : _data(data), _instruction(instruction)
{

}

Network::Packet::~Packet()
{

}

int Network::Packet::getInstruction() const
{
    return _instruction;
}

Stream &Network::Packet::getData()
{
    return _data;
}

Queue<Network::Packet> &Network::getQueueIn()
{
    return _queueIn;
}

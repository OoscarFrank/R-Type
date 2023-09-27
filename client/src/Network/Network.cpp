#include "Network.hpp"
#include <bitset>

Network::Network(std::string ip, int port) : _socket(_ioContext, asio::ip::udp::v4()), _serverEndpoint(asio::ip::address::from_string(ip), port)
{
    _ip = ip;
    _port = port;

    this->_ReaderThread = std::thread(&Network::read, this);

}

Network::~Network()
{
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
    asio::ip::udp::endpoint sender;
    std::pair<size_t, Stream> tmpInst;
    enum
    {
        max_length = 1024
    };
    char data[max_length];

    while (true)
    {
        asio::error_code ec;
        size_t len = this->_socket.receive_from(asio::buffer(data, max_length), _serverEndpoint, 0, ec);
        this->_streamIn.setDataCharArray(data, len);

        while ((tmpInst = this->getNextInst()).first != 0) {
            _queueIn.push(Network::Packet(tmpInst.second, tmpInst.first));
            // std::cout << tmpInst.first << tmpInst.second << std::endl;
        }
        if (ec) {
            Network::ReadError error;
            error._message = "Error during receive: " + ec.message();
            throw error;
        }
    }
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

#include "Network.hpp"

Network::Network(std::string ip, int port) : _socket(_ioContext, asio::ip::udp::v4()), _serverEndpoint(asio::ip::address::from_string(ip), port)
{
    _ip = ip;
    _port = port;

    this->_ReaderThread = std::thread(&Network::read, this);

}

Network::~Network()
{
}

void Network::catCharOut(const char &data)
{
    this->_dataOut += data;
}

void Network::catShortOut(const short &data)
{
    char tmp;
    this->_dataOut += data;
    tmp = data >> 8;
    this->_dataOut += tmp;
}

void Network::catIntOut(const int &data)
{
    char tmp = data;
    this->_dataOut += tmp;
    tmp = data >> 8;
    this->_dataOut += tmp;
    tmp = data >> 16;
    this->_dataOut += tmp;
    tmp = data >> 24;
    this->_dataOut += tmp;
}

void Network::setInst(unsigned char inst)
{
    _instOut = inst;
}

void Network::send(const std::string &message)
{
    if (_serverEndpoint.protocol() == asio::ip::udp::v4())
        _socket.send_to(asio::buffer(message), _serverEndpoint);
    else
        std::cerr << "Endpoint is not IPv4" << std::endl;
}

void Network::send()
{
    std::string out;
    out += _instOut;
    out += _dataOut;
    send(out);
    _dataOut = "";
    _instOut = 0;
}


void Network::read()
{
    asio::ip::udp::endpoint sender;
    std::pair<size_t, std::string> tmpInst;
    enum
    {
        max_length = 1024
    };
    char data[max_length];

    while (true)
    {
        asio::error_code ec;
        size_t len = this->_socket.receive_from(asio::buffer(data, max_length), _serverEndpoint, 0, ec);
        data[len] = '\0';

        while ((tmpInst = this->getNextInst()).first != 0)
            _queueIn.push(Network::Packet( tmpInst.second, tmpInst.first));
        if (ec) {
            Network::ReadError error;
            error._message = "Error during receive: " + ec.message();
            throw error;
        }
    }
}



std::pair<size_t, std::string> Network::getNextInst()
{
    if (this->_buferIn.size() == 0)
        return std::make_pair(0, "");
    std::vector<Commands> inst = OUT_COMMANDS;
    std::pair<size_t, std::string> out;

    for (auto i = inst.begin(); i != inst.end(); ++i) {
        if (this->_buferIn[0] == i->_inst) {
            if (this->_buferIn.size() < i->_size + 1)
                return std::make_pair(0, "");
            out.first = i->_inst;
            out.second = this->_buferIn.substr(1, i->_size);
            this->_buferIn = this->_buferIn.substr(i->_size + 1);
            return out;
        }
    }
    this->_buferIn = "";
    return std::make_pair(0, "");
}





Network::Packet::Packet(const std::string &data, int instruction) : _data(data), _instruction(instruction)
{

}

Network::Packet::~Packet()
{

}

int Network::Packet::getInstruction() const
{
    return _instruction;
}

const std::string &Network::Packet::getData() const
{
    return _data;
}

int Network::Packet::getDataInt()
{
    int out = 0;
    out += this->_data[0];
    out += this->_data[1] << 8;
    out += this->_data[2] << 16;
    out += this->_data[3] << 24;
    this->_data = this->_data.substr(4);
    return out;
}

short Network::Packet::getDataShort()
{
    short out = 0;
    out += this->_data[0];
    out += this->_data[1] << 8;
    this->_data = this->_data.substr(2);
    return out;
}

char Network::Packet::getDataChar()
{
    char out = 0;
    out += this->_data[0];
    this->_data = this->_data.substr(1);
    return out;
}

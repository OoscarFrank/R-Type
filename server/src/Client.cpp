#include "Client.hpp"
#include "Utils/Instruction.hpp"

Client::Client(asio::ip::udp::socket &socket, asio::ip::udp::endpoint endpoint): _socket(socket)
{
    this->_endpoint = endpoint;
    this->buffer = "";
    this->_dataOut = "";
    this->_instOut = 0;
    this->_roomId = 0;
    this->lastActivity = NOW;
}

Client::~Client()
{
}

const asio::ip::udp::endpoint &Client::getEndpoint() const
{
    return this->_endpoint;
}

void Client::pushBuffer(const std::string &data)
{
    this->buffer += data;
}

std::pair<size_t, std::string> Client::getNextInst()
{

    if (this->buffer.size() == 0)
        return std::make_pair(0, "");
    std::vector<Commands> inst = IN_COMMANDS;
    std::pair<size_t, std::string> out;

    for (auto i = inst.begin(); i != inst.end(); ++i) {
        if (this->buffer[0] == i->_inst) {
            if (this->buffer.size() < i->_size + 1)
                return std::make_pair(0, "");
            out.first = i->_inst;
            out.second = this->buffer.substr(1, i->_size);
            this->buffer = this->buffer.substr(i->_size + 1);
            return out;
        }
    }
    this->buffer = "";
    return std::make_pair(0, "");
}

void Client::catCharOut(const char &data)
{
    this->_dataOut += data;
}

void Client::catShortOut(const short &data)
{
    char tmp;
    this->_dataOut += data;
    tmp = data >> 8;
    this->_dataOut += tmp;
}

void Client::catIntOut(const int &data)
{
    char tmp = data;
    this->_dataOut += tmp;
    tmp = data >> 8;
    this->_dataOut += tmp;
    tmp = data >> 16;
    this->_dataOut += tmp;
    tmp = data >> 24;
    this->_dataOut += tmp;

    // int out = 0;
    // out += this->_dataOut[0];
    // out += this->_dataOut[1] << 8;
    // out += this->_dataOut[2] << 16;
    // out += this->_dataOut[3] << 24;
    // std::cout << out << std::endl;
}

void Client::setInst(unsigned char inst)
{
    _instOut = inst;
}

void Client::send()
{
    std::string out;
    out += _instOut;
    out += _dataOut;
    send(out);
    _dataOut = "";
    _instOut = 0;
}

void Client::send(const std::string &message)
{
    if (_endpoint.protocol() == asio::ip::udp::v4()) {
        _socket.send_to(asio::buffer(message), _endpoint);
    } else
        std::cerr << "Endpoint is not IPv4" << std::endl;
}

#include "Network.hpp"
#include <bitset>

using namespace TypesLitterals;

Network::Network(std::string ip, int port) : _socket(_ioContext, asio::ip::udp::v4()), _serverEndpoint(asio::ip::address::from_string(ip), port), _inCommands(IN_COMMANDS), _outCommands(OUT_COMMANDS)
{
    this->_ReaderThread = std::thread(&Network::read, this);
    _closed = false;
    _cmdNbr = 0;
    _lastCmdNbrRecieved = 0;
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

void Network::send(const Stream &stream)
{
    Stream tmp = stream;

    for (auto i = _outCommands.begin(); i != _outCommands.end(); ++i) {
        if (stream[0] == i->_inst) {
            if (stream.size() != i->_size + 1) {
                std::cout << "Bad size" << std::endl;
                return;
            }
            if (i->_important) {
                u_short nbr = ++_cmdNbr;
                _sentPackets[nbr] = stream;
                tmp.clear();
                tmp << stream[0] << nbr << stream.subStream(1);
            }
            break;
        }
    }

    if (_serverEndpoint.protocol() == asio::ip::udp::v4())
        _socket.send_to(asio::buffer(tmp.toString()), _serverEndpoint);
    else
        std::cerr << "Endpoint is not IPv4" << std::endl;
}

void Network::resend(u_short cmdNbr)
{
    std::cout << "Resending " << cmdNbr << std::endl;

    try {
        Stream out = _sentPackets.at(cmdNbr);
        send(out);
    } catch (const std::out_of_range &e) {
        std::cerr << "Packet " << cmdNbr << " not found" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
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
                while ((tmpInst = this->getNextInst()).first != 0) {
                    _queueIn.push(Network::Packet(tmpInst.second, tmpInst.first));
                }
            } else if (ec) {
                std::cerr << "Error in Network" << std::endl;
            }
            startReceive();
        }
    );
}

std::pair<size_t, Stream> Network::getNextInst()
{
    if (this->_streamIn.size() == 0)
        return std::make_pair(0, Stream());
    std::pair<size_t, Stream> out;

    for (auto i = _inCommands.begin(); i != _inCommands.end(); ++i) {
        try {
            if (this->_streamIn[0] == i->_inst) {
                u_int size = i->_important ? i->_size + 2 : i->_size;
                if (this->_streamIn.size() < size + 1)
                    return std::make_pair(0, Stream());
                out.first = i->_inst;
                out.second = this->_streamIn.subStream(i->_important ? 3 : 1, i->_size);
                if (i->_important) {
                    u_short test = _streamIn.subStream(1).getDataUShort();
                    int count = 0;
                    if (_lastCmdNbrRecieved < test) {
                        if (static_cast<int>(test) - static_cast<int>(_lastCmdNbrRecieved) > 50)
                            _lastCmdNbrRecieved = test - 1;
                    }
                    for (_lastCmdNbrRecieved++; test != _lastCmdNbrRecieved; _lastCmdNbrRecieved++) {
                        std::cout << "Cmd not recieved: " << _lastCmdNbrRecieved << std::endl;
                        Stream out;
                        out << 255_uc << _lastCmdNbrRecieved;
                        send(out);
                        ++count;
                        if (count > 50) {
                            std::cout << "Too many commands not recieved " << test << " < " << _lastCmdNbrRecieved << std::endl;
                            break;
                        }
                    }
                }
                this->_streamIn = this->_streamIn.subStream(size + 1);
                return out;
            }
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    this->_streamIn.clear();
    return std::make_pair(0, Stream());
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

Network::Packet &Network::Packet::operator>>(u_char &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(u_short &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(u_int &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(u_long &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(char &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(short &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(int &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(long &data)
{
    _data >> data;
    return *this;
}

Network::Packet &Network::Packet::operator>>(bool &data)
{
    _data >> data;
    return *this;
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

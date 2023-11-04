#include "Client.hpp"

Client::Client(asio::ip::udp::socket &socket, asio::ip::udp::endpoint endpoint): _socket(socket), _outCommands(OUT_COMMANDS), _inCommands(IN_COMMANDS)
{
    this->_endpoint = endpoint;
    this->_lastActivity = NOW;
    this->_cmdNbr = 0;
    this->_lastCmdNbrRecieved = 0;
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
    std::pair<size_t, Stream> out;

    for (auto i = _inCommands.begin(); i != _inCommands.end(); ++i) {
        if (this->_streamIn[0] == i->_inst) {
            u_int size = i->_important ? i->_size + 2 : i->_size;
            if (this->_streamIn.size() < size + 1)
                return std::make_pair(0, Stream());
            out.first = i->_inst;
            out.second = this->_streamIn.subStream(i->_important ? 3 : 1, i->_size);
            if (i->_important) {
                u_short test = _streamIn.subStream(1).getDataUShort();
                if (!_initialised && test != 1) {
                    _lastCmdNbrRecieved = test - 1;
                }
                _initialised = true;
                int count = 0;
                
                for (_lastCmdNbrRecieved++; test != _lastCmdNbrRecieved; _lastCmdNbrRecieved++) {
                    std::cout << "Cmd not recieved: " << _lastCmdNbrRecieved << std::endl;
                    send(StreamFactory::askResend(_lastCmdNbrRecieved));
                    ++count;
                    if (count > 50) {
                        std::cout << "Too many commands not recieved " << test << " " << _lastCmdNbrRecieved << std::endl;
                        
                        break;
                    }
                }
            }
            this->_streamIn = this->_streamIn.subStream(size + 1);
            return out;
        }
    }
    this->_streamIn.clear();
    return std::make_pair(0, Stream());
}

void Client::send(const Stream &stream)
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

    if (_endpoint.protocol() == asio::ip::udp::v4())
        _socket.send_to(asio::buffer(tmp.toString()), _endpoint);
    else
        std::cerr << "Endpoint is not IPv4" << std::endl;
}

void Client::resend(u_short cmdNbr)
{
    std::unique_lock<std::mutex> lock(_resendMutex);
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

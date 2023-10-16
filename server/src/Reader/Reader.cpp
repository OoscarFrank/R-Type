#include "Reader.hpp"

Reader::Reader(asio::ip::udp::socket &socket, Queue<Reader::Packet> &queueIn, std::vector<std::shared_ptr<Client>> &clients): _socket(socket), _queueIn(queueIn), _clients(clients)
{
    _thread = std::thread(&Reader::Clock, this);
}

Reader::~Reader()
{
    _thread.join();
}

void Reader::Clock()
{
    asio::ip::udp::endpoint sender;
    std::pair<size_t, Stream> tmpInst;
    const size_t max_length = 1024;
    char data[max_length];

    while (true)
    {
        bool pass = false;
        asio::error_code ec;
        size_t len = this->_socket.receive_from(asio::buffer(data, max_length), sender, 0, ec);

        if (len == 0)
            continue;

        for (auto i = this->_clients.begin(); i != this->_clients.end(); i++)
        {
            if ((**i).getEndpoint() == sender) {
                pass = true;
                (**i).getStreamIn().setDataCharArray(data, len);
                while ((tmpInst = (**i).getNextInst()).first != 0) {
                    _queueIn.push(Reader::Packet(*i, tmpInst.second, tmpInst.first));
                }
                break;
            }
        }

        if (!pass)
        {
            this->_clients.push_back(std::make_shared<Client>(_socket, sender));
            this->_clients.back()->getStreamIn().setDataCharArray(data, len);
            while ((tmpInst = this->_clients.back()->getNextInst()).first != 0) {
                _queueIn.push(Reader::Packet(this->_clients.back(), tmpInst.second, tmpInst.first));
            }
        }

        if (ec)
        {
            Reader::ReadError error;
            error._message = "Error during receive: " + ec.message();
            throw error;
        }
    }
}

Reader::Packet::Packet(std::shared_ptr<Client> client, const Stream &data, int instruction) : _client(client), _data(data), _instruction(instruction)
{
}

Reader::Packet::~Packet()
{
}

int Reader::Packet::getInstruction() const
{
    return _instruction;
}

std::shared_ptr<Client> Reader::Packet::getClient() const
{
    return _client;
}

Stream &Reader::Packet::getData()
{
    return _data;
}

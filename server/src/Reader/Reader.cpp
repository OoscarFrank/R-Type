#include "Reader.hpp"

Reader::Reader(asio::ip::udp::socket &socket, Queue<std::unique_ptr<Reader::Packet>> &queueIn, std::vector<std::unique_ptr<Client>> &clients) : _socket(socket), _queueIn(queueIn), _clients(clients)
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
    bool pass = false;
    std::pair<size_t, std::string> tmpInst;
    enum
    {
        max_length = 1024
    };
    char data[max_length];

    while (true)
    {
        pass = false;
        std::error_code ec;
        this->_socket.receive_from(asio::buffer(data, max_length), sender, 0, ec);

        for (auto i = this->_clients.begin(); i != this->_clients.end(); i++)
        {
            if ((*i)->getEndpoint() == sender)
            {
                pass = true;
                (*i)->pushBuffer(std::string(data));
                tmpInst = (*i)->getNextInst();
                

                if (tmpInst.first != 0)
                    _queueIn.push(std::make_unique<Reader::Packet>(**i, tmpInst.second, tmpInst.first));
                break;
            }
        }

        if (!pass)
        {
            this->_clients.push_back(std::make_unique<Client>(sender));
            this->_clients.back()->pushBuffer(std::string(data));
            tmpInst = this->_clients.back()->getNextInst();

            if (tmpInst.first != 0)
                _queueIn.push(std::make_unique<Reader::Packet>(*(this->_clients.back()), tmpInst.second, tmpInst.first));
        }

        if (ec)
        {
            Reader::ReadError error;
            error._message = "Error during receive: " + ec.message();
            throw error;
        }
    }
}

Reader::Packet::Packet(Client &client, const std::string &data, int instruction) : _client(client), _data(data), _instruction(instruction)
{
}

Reader::Packet::~Packet()
{
}

int Reader::Packet::getInstruction() const
{
    return _instruction;
}

const std::string &Reader::Packet::getData() const
{
    return _data;
}

const Client &Reader::Packet::getClient() const
{
    return _client;
}
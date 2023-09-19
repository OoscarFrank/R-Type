#include "Client.hpp"

Client::Client(asio::ip::udp::endpoint endpoint)
{
    this->_endpoint = endpoint;
    this->buffer = "";
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
    
    if (this->buffer[0] == '1') {
        this->buffer.erase(0, 1);
        return std::make_pair(1, "oui");
    }

    return std::make_pair(0, "");

}
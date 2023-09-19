#include "Client.hpp"

Client::Client(asio::ip::udp::endpoint endpoint)
{
    this->_endpoint = endpoint;
}

Client::~Client()
{
}

const asio::ip::udp::endpoint &Client::GetEndpoint() const
{
    return this->_endpoint;
}

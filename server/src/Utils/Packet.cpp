#include "Packet.hpp"

Packet::Packet(const asio::ip::udp::endpoint &endpoint, const std::string &message): _endpoint(endpoint), _message(message) {}

const asio::ip::udp::endpoint &Packet::endpoint() const {
    return _endpoint;
}

const std::string &Packet::message() const {
    return _message;
}

void Packet::setMessage(const std::string &message) {
    _message = message;
}

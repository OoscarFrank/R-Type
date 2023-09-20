#include "Writer.hpp"

Writer::Writer(asio::ip::udp::socket &socket, Queue<Writer::Packet> &queue): _socket(socket), _queue(queue) {
    _thread = std::thread(&Writer::run, this);
}

Writer::~Writer() {
    _thread.join();
}

void Writer::run()
{
    while (true) {
        Writer::Packet value = _queue.pop();
        try {
            _socket.send_to(asio::buffer(value.message()), value.endpoint());
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

Writer::Packet::Packet(const asio::ip::udp::endpoint &endpoint, const std::string &message): _endpoint(endpoint), _message(message) {}

const asio::ip::udp::endpoint &Writer::Packet::endpoint() const {
    return _endpoint;
}

const std::string &Writer::Packet::message() const {
    return _message;
}

void Writer::Packet::setMessage(const std::string &message) {
    _message = message;
}

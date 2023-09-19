#include "Reader.hpp"



Reader::Reader(asio::ip::udp::socket &socket, Queue<Packet> &queueIn): _socket(socket), _queueIn(queueIn)
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
    enum { max_length = 1024 };
    char data[max_length];

    while (true) {
        std::error_code ec;
        this->_socket.receive_from(asio::buffer(data, max_length), sender, 0, ec);

        _queueIn.push(Packet(sender, std::string(data)));
        if (ec) {
            Reader::ReadError error;
            error._message = "Error during receive: " + ec.message();
            throw error;
        }
    }
}

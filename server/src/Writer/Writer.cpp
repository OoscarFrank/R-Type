#include "Writer.hpp"

Writer::Writer(asio::ip::udp::socket &socket, Queue<Packet> &queue): _socket(socket), _queue(queue) {
    _thread = std::thread(&Writer::run, this);
}

Writer::~Writer() {
    _thread.join();
}

void Writer::run()
{
    while (true) {
        Packet value = _queue.pop();
        _socket.send_to(asio::buffer(value.message()), value.endpoint());
    }
}

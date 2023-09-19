#include <iostream>
#include <asio.hpp>
#include "Writer/Writer.hpp"
// #include "Writer/Packet.hpp"
#include "Reader/Reader.hpp"

int main(int argc, char **argv)
{
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 4242));
    Queue<Packet> queueOut;
    Queue<Packet> queueIn;
    std::vector<std::unique_ptr<Client>> clients;
    Reader reader(socket, queueIn);
    Writer writer(socket, queueOut);
    return 0;
}

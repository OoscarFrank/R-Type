#include <iostream>
#include <asio.hpp>
#include "Writer/Writer.hpp"
// #include "Writer/Packet.hpp"
#include "Reader/Reader.hpp"
#include "Utils/ThreadPool.hpp"

int main(int argc, char **argv)
{
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 4242));
    Queue<Packet> queueOut;
    Queue<std::unique_ptr<Reader::Packet>> queueIn;
    std::vector<std::unique_ptr<Client>> clients;
    Reader reader(socket, queueIn, clients);
    Writer writer(socket, queueOut);

    // ThreadPool pool(3, 10);
    // while (true) {
    //     Packet value = queueIn.pop();
    //     pool.submit([&value, &queueOut]() {
    //         queueOut.push(Packet(value.endpoint(), "J'AI RECU TON MESSAGE: " + value.message()));
    //     });
    // }
    return 0;
}

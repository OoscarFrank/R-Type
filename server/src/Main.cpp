#include <iostream>
#include <asio.hpp>
#include "Writer/Writer.hpp"
#include "Reader/Reader.hpp"
#include "Utils/ThreadPool.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    const int port = 4242;
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
    Queue<Writer::Packet> queueOut;
    Queue<Reader::Packet> queueIn;
    std::vector<Client> clients;
    Reader reader(socket, queueIn, clients);
    Writer writer(socket, queueOut);
    std::cout << "Server listening on port " << port << std::endl;

    ThreadPool pool(3, 10);
    while (true) {
        Reader::Packet value = queueIn.pop();
        pool.submit([value, &queueOut]() {
            queueOut.push(Writer::Packet(value.getClient().getEndpoint(), std::string("J'AI RECU TON MESSAGE: ") + value.getData()));
        });
    }
    return 0;
}

#include <iostream>
#include <asio.hpp>
#include "Writer/Writer.hpp"
#include "Reader/Reader.hpp"
#include "Utils/ThreadPool.hpp"
#include "Utils/Args.hpp"

void exec(int port)
{
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
}

int main(int argc, char **argv)
{
    try {
        Args args(argc, argv);
        exec(args.getFlagValue<int>("-p", 4242));
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}

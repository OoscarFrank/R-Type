#include <iostream>
#include <asio.hpp>
#include "Reader/Reader.hpp"
#include "Game/RoomManager.hpp"
#include "Utils/ThreadPool.hpp"
#include "Utils/Args.hpp"
#include "Utils/Instruction.hpp"
#include "Router.hpp"

void exec(int port)
{
    std::cout << "starting server..." << std::endl;
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
    Queue<Reader::Packet> queueIn;
    std::vector<std::shared_ptr<Client>> clients;
    Reader reader(socket, queueIn, clients);
    RoomManager rm;
    Router router(rm);
    Levels levels;

    std::cout << "Server listening on port " << port << std::endl;

    int nbThread = std::thread::hardware_concurrency() - 3;
    ThreadPool reqPool(nbThread / 2, 10);
    while (true) {
        Reader::Packet value = queueIn.pop();
        reqPool.submit([value, &router, &levels]() {
            router.route(value, levels);
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

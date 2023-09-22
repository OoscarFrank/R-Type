#include <iostream>
#include <asio.hpp>
#include "Reader/Reader.hpp"
#include "Game/Game.hpp"
#include "Utils/ThreadPool.hpp"
#include "Utils/Args.hpp"

void router(Reader::Packet packet, Game &game, asio::ip::udp::socket &socket)
{
    switch (packet.getInstruction()) {
        case 5:
            try {
                Room &room = game.getRoom(packet.getClient());
                room.getPlayer(packet.getClient()).fireMissile();
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
            break;
        case 8:
            game.createRoom(packet, socket, ((packet.getDataChar() == 1) ? true : false));
            break;
        case 9:
            game.searchRoom(packet, socket);
            break;
        default:
            break;
    }
}

void exec(int port)
{
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
    Queue<Reader::Packet> queueIn;
    std::vector<std::shared_ptr<Client>> clients;
    Reader reader(socket, queueIn, clients);
    Game game;

    std::cout << "Server listening on port " << port << std::endl;

    int nbThread = std::thread::hardware_concurrency() - 3;
    ThreadPool reqPool(nbThread / 2, 10);
    while (true) {
        Reader::Packet value = queueIn.pop();
        reqPool.submit([value, &socket, &game]() {
            router(value, game, socket);
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

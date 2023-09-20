#include <iostream>
#include <asio.hpp>
#include "Reader/Reader.hpp"
#include "Game/Game.hpp"
#include "Utils/ThreadPool.hpp"
#include "Utils/Args.hpp"

void router(Reader::Packet packet, Game &game, asio::ip::udp::socket &socket)
{

    std::cout << packet.getInstruction() << std::endl;
    switch (packet.getInstruction()) {
        case 8:
            game.createRoom(packet.getClient());
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
    std::vector<Client> clients;
    Reader reader(socket, queueIn, clients);
    Game game;

    std::cout << "Server listening on port " << port << std::endl;

    ThreadPool pool(std::thread::hardware_concurrency() - 3, 10);
    while (true) {
        Reader::Packet value = queueIn.pop();
        pool.submit([value, &socket, &game]() {
            // value.getClient().catIntOut(4865);
            router(value, game, socket);
            socket.send_to(asio::buffer("JAI EU LE MESSAGE"), value.getClient().getEndpoint());
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

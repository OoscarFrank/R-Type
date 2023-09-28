#include <iostream>
#include <asio.hpp>
#include "Reader/Reader.hpp"
#include "Game/Game.hpp"
#include "Utils/ThreadPool.hpp"
#include "Utils/Args.hpp"
#include "Utils/Instruction.hpp"

void router(Reader::Packet packet, Game &game)
{
    try {
        switch (packet.getInstruction()) {
            case 2:
                {
                    char move = packet.getData().getDataChar();
                    char nbr = packet.getData().getDataChar();
                    game.getRoom(packet.getClient()).movePlayer(packet.getClient(), move, nbr);
                }
                break;
            case 5:
                {
                Room &tmpRoom = game.getRoom(packet.getClient());
                tmpRoom.getPlayer(packet.getClient()).fireMissile(tmpRoom.getMissilesIds());
                }
                break;
            case 8:
                game.createRoom(packet, ((packet.getData().getDataChar() == 1) ? true : false));
                break;
            case 9:
                game.searchRoom(packet);
                break;
            case 12:
                packet.getClient()->ping();
                break;
            default:
                break;
        }
    } catch (const std::exception &e) {
        std::cerr << "[ROUTER ERROR] " << e.what() << std::endl;
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
        reqPool.submit([value, &game]() {
            router(value, game);
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

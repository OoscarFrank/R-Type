#include <iostream>
#include <asio.hpp>
#include "Reader/Reader.hpp"
#include "Game/Game.hpp"
#include "Utils/ThreadPool.hpp"
#include "Utils/Args.hpp"
#include "Utils/Instruction.hpp"

void router(Reader::Packet packet, Game &game, asio::ip::udp::socket &socket)
{
    try {
        switch (packet.getInstruction()) {
            case 2:
                {
                    char move = packet.getDataChar();
                    char nbr = packet.getDataChar();
                    Player &player = game.getRoom(packet.getClient()).getPlayer(packet.getClient());
                    for (int i = 0; i < nbr; i++) {
                        if (move == PLAYER_MOVE_UP)
                            player.move(-PLAYER_MOVE_OFFSET, 0);
                        else if (move == PLAYER_MOVE_DOWN)
                            player.move(PLAYER_MOVE_OFFSET, 0);
                        else if (move == PLAYER_MOVE_LEFT)
                            player.move(0, -PLAYER_MOVE_OFFSET);
                        else if (move == PLAYER_MOVE_RIGHT)
                            player.move(0, PLAYER_MOVE_OFFSET);
                    }
                }
                break;
            case 5:
                game.getRoom(packet.getClient()).getPlayer(packet.getClient()).fireMissile();
                break;
            case 8:
                game.createRoom(packet, ((packet.getDataChar() == 1) ? true : false));
                break;
            case 9:
                game.searchRoom(packet);
                break;
            default:
                break;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
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

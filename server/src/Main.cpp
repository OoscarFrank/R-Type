#include <iostream>
#include <asio.hpp>
#include "Reader/Reader.hpp"
#include "Game/RoomManager.hpp"
#include "Utils/ThreadPool.hpp"
#include "Utils/Args.hpp"
#include "Utils/Instruction.hpp"
#include "Router.hpp"

void exec(int port, std::vector<std::string> stages)
{
    std::cout << "starting server..." << std::endl;
    if (stages.empty()) {
        throw std::runtime_error("No stages given");
    }

    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
    Queue<Reader::Packet> queueIn;
    std::vector<std::shared_ptr<Client>> clients;
    Reader reader(socket, queueIn, clients);
    RoomManager rm;
    Router router(rm, clients);
    Levels levels(stages);

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

void printHelp()
{
    std::cout << "USAGE: ./r-type_server -p port -s stage1 stage2 ..." << std::endl;
    std::cout << "\tport\t\tis the port number (default : 4242)" << std::endl;
    std::cout << "\tstages\t\tare the paths of the scripts files" << std::endl;
}

int main(int argc, char **argv)
{
    try {
        Args args(argc, argv);

        if (args.isFlagSet("--help") || args.isFlagSet("-help") || args.isFlagSet("-h")) {
            printHelp();
            return 0;
        }
        std::vector<std::string> defaultStages = {"./stages/stage1.script","./stages/stage2.script","./stages/stage3.script","./stages/stage4.script","./stages/stage5.script","./stages/stage6.script", "./stages/stage7.script"};
        exec(args.getFlagValue<int>("-p", 4242), args.getFlagValues<std::string>("-s", defaultStages));
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}

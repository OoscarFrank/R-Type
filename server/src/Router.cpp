#include "Router.hpp"

using namespace TypesLitterals;

Router::Router(RoomManager &rm):
    _rm(rm)
{
    _functions[2] = &Router::_movePlayer;
    _functions[5] = &Router::_fireMissile;
    _functions[8] = &Router::_createRoom;
    _functions[9] = &Router::_searchRoom;
    _functions[12] = &Router::_ping;
    _functions[24] = &Router::_leaveRoom;
    _functions[255] = &Router::_cmdNotRecieved;
}

void Router::route(Reader::Packet packet, Levels &levels)
{
    try {
        packet.getClient()->ping();
        (this->*_functions.at(packet.getInstruction()))(packet, levels);
    } catch (const std::exception &e) {
        std::cerr << "[ROUTER ERROR] " << e.what() << std::endl;
    }
}

void Router::_movePlayer(Reader::Packet &packet, Levels &levels)
{
    char move;
    char nbr;
    packet >> move >> nbr;
    _rm.getRoom(packet.getClient()).movePlayer(packet.getClient(), move, nbr);
}

void Router::_fireMissile(Reader::Packet &packet, Levels &levels)
{
    Room &tmpRoom = _rm.getRoom(packet.getClient());
    tmpRoom.getPlayer(packet.getClient()).fireMissile();
}

void Router::_createRoom(Reader::Packet &packet, Levels &levels)
{
    bool isPrivate;
    packet >> isPrivate;
    _rm.createRoom(packet, levels, isPrivate);
}

void Router::_searchRoom(Reader::Packet &packet, Levels &levels)
{
    _rm.searchRoom(packet, levels);
}

void Router::_ping(Reader::Packet &packet, Levels &levels)
{
    long timeMS;
    packet >> timeMS;

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point then = std::chrono::system_clock::time_point(std::chrono::milliseconds(timeMS));

    Stream out;
    out << 23_uc << static_cast<u_short>(std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count());
    packet.getClient()->send(out);
}

void Router::_leaveRoom(Reader::Packet &packet, Levels &levels)
{
    auto client = packet.getClient();
    _rm.getRoom(client).removePlayer(client);
}

void Router::_cmdNotRecieved(Reader::Packet &packet, Levels &levels)
{
    u_short cmdNbr;
    packet >> cmdNbr;
    packet.getClient()->resend(cmdNbr);
}

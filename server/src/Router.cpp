#include "Router.hpp"

Router::Router(RoomManager &rm):
    _rm(rm)
{
    _functions[2] = &Router::_movePlayer;
    _functions[5] = &Router::_fireMissile;
    _functions[8] = &Router::_createRoom;
    _functions[9] = &Router::_searchRoom;
    _functions[12] = &Router::_ping;
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
    char move = packet.getData().getDataChar();
    char nbr = packet.getData().getDataChar();
    _rm.getRoom(packet.getClient()).movePlayer(packet.getClient(), move, nbr);
}

void Router::_fireMissile(Reader::Packet &packet, Levels &levels)
{
    Room &tmpRoom = _rm.getRoom(packet.getClient());
    tmpRoom.getPlayer(packet.getClient()).fireMissile();
}

void Router::_createRoom(Reader::Packet &packet, Levels &levels)
{
    _rm.createRoom(packet, levels, ((packet.getData().getDataChar() == 1) ? true : false));
}

void Router::_searchRoom(Reader::Packet &packet, Levels &levels)
{
    _rm.searchRoom(packet, levels);
}

void Router::_ping(Reader::Packet &packet, Levels &levels)
{}

void Router::_cmdNotRecieved(Reader::Packet &packet, Levels &levels)
{
    packet.getClient()->resend(packet.getData().getDataUShort());
}

#include "Router.hpp"

using namespace TypesLitterals;

Router::Router(RoomManager &rm, const std::vector<std::shared_ptr<Client>> &allClients):
    _rm(rm),
    _allClients(allClients)
{
    _functions[2] = &Router::_movePlayer;
    _functions[5] = &Router::_fireMissile;
    _functions[8] = &Router::_createRoom;
    _functions[9] = &Router::_searchRoom;
    _functions[12] = &Router::_ping;
    _functions[24] = &Router::_leaveRoom;
    _functions[25] = &Router::_joinRoom;
    _functions[26] = &Router::_listRooms;
    _functions[30] = &Router::_fireBomb;
    _functions[34] = &Router::_sendChat;
    _functions[36] = &Router::_fireLaser;
    _functions[38] = &Router::_fireRay;
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
    _rm.createRoom(packet, levels, _allClients, isPrivate);
}

void Router::_searchRoom(Reader::Packet &packet, Levels &levels)
{
    _rm.searchRoom(packet, levels, _allClients);
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

void Router::_joinRoom(Reader::Packet &packet, Levels &levels)
{
    u_int roomId;
    packet >> roomId;
    _rm.getRoom(roomId).addPlayer(packet.getClient());
}

void Router::_listRooms(Reader::Packet &packet, Levels &levels)
{
    const auto &rooms = _rm.getRooms();
    for (auto i = rooms.begin(); i != rooms.end(); i++) {
        if ((*i)->isPrivate() || (*i)->getState() != Room::WAIT)
            continue;
        Stream out;
        out << 27_uc << (*i)->getId() << static_cast<u_char>((*i)->getNbPlayer()) << static_cast<u_char>((*i)->getMaxPlayer()) << 1_uc;
        packet.getClient()->send(out);
    }
}

void Router::_sendChat(Reader::Packet &packet, Levels &levels)
{
    std::string msg;
    char tmp;

    for (int i = 0; i < 1000; ++i) {
        packet >> tmp;
        if (tmp == 0)
            break;
        msg += tmp;
    }

    Room &room = _rm.getRoom(packet.getClient());
    room.sendChat(packet.getClient(), msg);
}

void Router::_cmdNotRecieved(Reader::Packet &packet, Levels &levels)
{
    u_short cmdNbr;
    packet >> cmdNbr;
    packet.getClient()->resend(cmdNbr);
}

void Router::_fireBomb(Reader::Packet &packet, Levels &levels)
{
    Room &tmpRoom = _rm.getRoom(packet.getClient());
    try {

        Player &tmpPlayer = tmpRoom.getPlayer(packet.getClient());
        tmpRoom._playersMutex.lock();
        ForcePod &tmp = tmpPlayer.forcePod();
        tmp.shootBomb();
        tmpRoom._playersMutex.unlock();
    } catch (const std::runtime_error &e) {
        tmpRoom._playersMutex.unlock();
        std::cerr << "error" << std::endl;;
    }
}

void Router::_fireLaser(Reader::Packet &packet, Levels &levels)
{
    Room &tmpRoom = _rm.getRoom(packet.getClient());
    try {
        Player &tmpPlayer = tmpRoom.getPlayer(packet.getClient());
        tmpRoom._playersMutex.lock();
        ForcePod &tmp = tmpPlayer.forcePod();
        tmp.shootLaser();
        tmpRoom._playersMutex.unlock();
    } catch (const std::runtime_error &e) {
        tmpRoom._playersMutex.unlock();
        std::cerr << "error" << std::endl;;
    }
}

void Router::_fireRay(Reader::Packet &packet, Levels &levels)
{
    Room &tmpRoom = _rm.getRoom(packet.getClient());
    try {
        Player &tmpPlayer = tmpRoom.getPlayer(packet.getClient());
        tmpRoom._playersMutex.lock();
        ForcePod &tmp = tmpPlayer.forcePod();
        tmp.shootRay();
        tmpRoom._playersMutex.unlock();
    } catch (const std::runtime_error &e) {
        tmpRoom._playersMutex.unlock();
        std::cerr << "error" << std::endl;;
    }
}
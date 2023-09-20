#include "Room.hpp"

Room::Room(unsigned int id, Client &client, bool privateRoom)
{
    _id = id;
    _nbPlayer = 1;
    _playersIds = 1;
    _maxPlayer = 4;
    _progress = 0;
    client.setGamePlayerId(_playersIds);
    _playersIds++;
    _clients.push_back(client);
    _private = privateRoom;

    // std::cout << "New room created with:" << std::endl;
    // for (auto i = _clients.begin(); i != _clients.end(); i++) {
    //     std::cout << i->getEndpoint().address() << std::endl;
    // }
}

Room::~Room()
{

}

unsigned short Room::getId() const
{
    return _id;
}

unsigned int Room::getNbPlayer() const
{
    return _nbPlayer;
}

unsigned int Room::getProgress() const
{
    return _progress;
}

unsigned int Room::getMaxPlayer() const
{
    return _maxPlayer;
}

void Room::addPlayer(Client &client)
{
    _clients.push_back(client);
    _nbPlayer++;
    client.setGamePlayerId(_playersIds);
    _playersIds++;
}

void Room::removePlayer(Client &client)
{
    for (auto i = _clients.begin(); i != _clients.end(); i++) {
        if (i->getEndpoint() == client.getEndpoint()) {
            _clients.erase(i);
            _nbPlayer--;
            break;
        }
    }
}


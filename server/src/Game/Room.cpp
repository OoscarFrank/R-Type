#include "Room.hpp"

Room::Room(unsigned int id, Client &client)
{
    _id = id;
    _nbPlayer = 1;
    _progress = 0;
    _clients.push_back(client);
}

Room::~Room()
{

}

unsigned int Room::getId() const
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

void Room::addPlayer(Client &client)
{
    _clients.push_back(client);
    _nbPlayer++;
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
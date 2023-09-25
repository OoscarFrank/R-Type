#include "Room.hpp"

Room::Room(unsigned int id, std::shared_ptr<Client> client, bool privateRoom)
{
    _id = id;
    _nbPlayer = 0;
    _playersIds = 1;
    _maxPlayer = 4;
    _progress = 0;

    addPlayer(client);
    _private = privateRoom;

    _thread = std::thread(&Room::refresh, this);

    // std::cout << "New room created with:" << std::endl;
    // for (auto i = _clients.begin(); i != _clients.end(); i++) {
    //     std::cout << i->getEndpoint().address() << std::endl;
    // }
}

Room::~Room()
{
    if (_thread.joinable())
        _thread.join();
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

void Room::addPlayer(std::shared_ptr<Client> client)
{
    for (auto i = _players.begin(); i != _players.end(); i++)
        if ((**i).client() == client)
            return;

    client->setInst(0x0a);
    client->catShortOut(_id);
    client->catCharOut(_playersIds);
    _players.push_back(std::make_unique<Player>(*this, &Room::sendToAll, client, _playersIds++, 100));
    _nbPlayer++;
    client->send();
}

void Room::movePlayer(std::shared_ptr<Client> client, char move, char nbr)
{
    Player &player = getPlayer(client);
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

void Room::removePlayer(std::shared_ptr<Client> client)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            _players.erase(i);
            _nbPlayer--;
            break;
        }
    }
}

bool Room::isClientInRoom(std::shared_ptr<Client> client)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            return true;
        }
    }
    return false;
}

Player &Room::getPlayer(std::shared_ptr<Client> client)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            return **i;
        }
    }
    throw std::runtime_error("Player not found");
}

void Room::sendToAll(const std::string &message)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        (**i).client()->send(message);
    }
}

void Room::refresh()
{
    while (true) {
        std::chrono::system_clock::time_point begin = std::chrono::system_clock::now();
        updateEntities();
        sendUpdateClients();
        std::cout << "Room " << _id << " has " << _nbPlayer << " players" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1) - (std::chrono::system_clock::now() - begin));
    }
}

void Room::updateEntities()
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        (**i).refreshMissiles();
    }
}

void Room::sendUpdateClients()
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        sendToAll("Player pos: " + std::to_string((**i).id()) + " " + std::to_string((**i).position().first) + " " + std::to_string((**i).position().second));
        for (auto j = (**i).missiles().begin(); j != (**i).missiles().end(); j++) {
            sendToAll("Player missiles: " + std::to_string((**i).id()) + " " + std::to_string(j->first) + " " + std::to_string(j->second));
        }
    }

}
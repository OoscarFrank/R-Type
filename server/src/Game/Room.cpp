#include "Room.hpp"

Room::Room(asio::ip::udp::socket &socket, unsigned int id, Client &client, bool privateRoom): _socket(socket)
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

void Room::refresh()
{
    while (true) {
        std::chrono::system_clock::time_point begin = std::chrono::system_clock::now();
        for (auto i = _clients.begin(); i != _clients.end(); i++) {
            _socket.send_to(asio::buffer("test"), i->getEndpoint());
        }
        std::cout << "Room " << _id << " has " << _nbPlayer << " players" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1) - (std::chrono::system_clock::now() - begin));
    }
}

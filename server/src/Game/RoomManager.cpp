#include "RoomManager.hpp"

RoomManager::RoomManager()
{
    _roomIds = 1;
}

RoomManager::~RoomManager()
{
}

void RoomManager::_clearRooms()
{
    std::unique_lock<std::mutex> lock(_roomsMutex);

    for (auto i = this->_rooms.begin(); i != this->_rooms.end();) {
        if ((**i).getState() == Room::STOPPED) {
            std::cout << "Room " << (**i).getId() << " closed" << std::endl;
            i = _rooms.erase(i);
        } else
            i++;
    }
}

void RoomManager:: createRoom(Reader::Packet &packet, Levels &levels, const std::vector<std::shared_ptr<Client>> &allClients, bool privateRoom)
{
    _clearRooms();
    std::unique_ptr<Room> newRoom = std::make_unique<Room>(_roomIds++, packet.getClient(), levels, allClients, privateRoom);
    std::cout << "New room: " << newRoom->getId() << std::endl;
    _roomsMutex.lock();
    _rooms.push_back(std::move(newRoom));
    _roomsMutex.unlock();
}

void RoomManager::searchRoom(Reader::Packet &packet, Levels &levels, const std::vector<std::shared_ptr<Client>> &allClients)
{
    _clearRooms();
    _roomsMutex.lock();
    if (_rooms.size() == 0) {
        _roomsMutex.unlock();
        this->createRoom(packet, levels, allClients);
        return;
    }

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if ((**i).getNbPlayer() < (**i).getMaxPlayer() && !(**i).isPrivate() && (**i).getState() == Room::WAIT) {
            (**i).addPlayer(packet.getClient());
            _roomsMutex.unlock();
            return;
        }
    }
    _roomsMutex.unlock();
    this->createRoom(packet, levels, allClients);
}

Room &RoomManager::getRoom(u_int id)
{
    _clearRooms();
    std::unique_lock<std::mutex> lock(_roomsMutex);

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if ((**i).getId() == id) {
            return **i;
        }
    }
    throw std::runtime_error("Room not found");
}

Room &RoomManager::getRoom(std::shared_ptr<Client> client)
{
    _clearRooms();
    std::unique_lock<std::mutex> lock(_roomsMutex);

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if ((**i).isClientInRoom(client)) {
            return **i;
        }
    }
    throw std::runtime_error("Room not found");
}

const std::vector<std::unique_ptr<Room>> &RoomManager::getRooms()
{
    _clearRooms();
    return _rooms;
}

#ifndef ROOM_MANAGER_HPP
#define ROOM_MANAGER_HPP

#include <iostream>
#include "Room.hpp"
#include <vector>
#include "../Reader/Reader.hpp"
#include <mutex>

class RoomManager
{
    private:
        std::vector<std::unique_ptr<Room>> _rooms;
        u_int _roomIds;
        std::mutex _roomsMutex;

        void _clearRooms();

    public:
        /**
         * @brief Construct a new RoomManager object
         *
         */
        RoomManager();
        ~RoomManager();
        /**
         * @brief Create a new room with a client (player)
         *
         * @param packet The packet received from the client
         * @param privateRoom If the room is private or not
         */
        void createRoom(Reader::Packet &packet, Levels &levels , bool privateRoom = false);
        /**
         * @brief Search a room for the client
         * If no room is found or all rooms are full, it will create a new one
         *
         * @param packet The packet received from the client
         */
        void searchRoom(Reader::Packet &packet, Levels &levels);

        /**
         * @brief Get a room by its id
         *
         * @param id The id of the room to find
         * @return Room&
         */
        Room &getRoom(u_int id);
        /**
         * @brief Get a room by a client that is in it
         *
         * @param client The client to find
         * @return Room&
         */
        Room &getRoom(std::shared_ptr<Client> client);
};

#endif
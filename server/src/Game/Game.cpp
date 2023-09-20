#include "Game.hpp"

Game::Game()
{
    _roomIds = 1;
}

Game::~Game()
{
}

void Game::createRoom(Client &client)
{
    Room room(_roomIds, client);
    client.setRoomId(_roomIds);
    _roomIds++;
    _rooms.push_back(room);
}
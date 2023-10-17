#include "Player.hpp"
#include "../Room.hpp"

Player::Player(Room &room, std::shared_ptr<Client> client, u_int id, short x, short y):
    ArmedEntity(room, id, x, y, PLAYER_WIDTH, PLAYER_HEIGHT),
    _score(0),
    _client(client)
{
    _room.sendToAll(StreamFactory::playerJoinedGame(_id));
    sendPos();
}

Player::Player(Room &room, std::shared_ptr<Client> client, u_int id, const std::pair<short, short> &pos):
    ArmedEntity(room, id, pos, {PLAYER_WIDTH, PLAYER_HEIGHT}),
    _score(0),
    _client(client)
{
    _room.sendToAll(StreamFactory::playerJoinedGame(_id));
    sendPos();
}

Player::~Player()
{

}

void Player::refresh()
{
    refreshMissiles();
}

bool Player::collide(const IEntity &other)
{
    return missilesCollide(other);
}

void Player::move(short dx, short dy)
{
    if (dx == 0 && dy == 0)
        return;
    if (_box.x + dx < 0 || _box.x + dx > SCREEN_WIDTH - _box.width || _box.y + dy < 0 || _box.y + dy > SCREEN_HEIGHT - _box.height)
        return;
    AEntity::move(dx, dy);
}

void Player::sendPos()
{
    _room.sendToAll(StreamFactory::playerPos(_id, _box.x, _box.y));
}

void Player::fireMissile()
{
    auto now = std::chrono::system_clock::now();


    if (_exist && std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= PLAYER_FIRE_TIME) {
        ArmedEntity::fireMissile(Missile::Type::PLAYER);
        _lastFire = now;
    }
}

const std::chrono::system_clock::time_point &Player::lastMoveTime() const
{
    return _lastMove;
}

void Player::setLastMoveTime(const std::chrono::system_clock::time_point &lastMove)
{
    _lastMove = lastMove;
}

int Player::score() const
{
    return _score;
}

void Player::setScore(int score)
{
    _score = score;
    _room.sendToAll(StreamFactory::score(_score));
}

void Player::setLife(int life)
{
    AEntity::setLife(life);
    _client->send(StreamFactory::playerLife(_life));
}

std::shared_ptr<Client> Player::client() const
{
    return _client;
}

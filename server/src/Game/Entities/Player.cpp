#include "Player.hpp"
#include "../Room.hpp"

Player::Player(Room &room, std::shared_ptr<Client> client, int id, short x, short y):
    ArmedEntity(room, id, x, y),
    _score(0),
    _client(client)
{
    Stream out;
    out.setDataUChar(13);
    out.setDataChar(static_cast<u_char>(_id));
    _room.sendToAll(out);
    sendPos();
}

Player::Player(Room &room, std::shared_ptr<Client> client, int id, const std::pair<short, short> &pos):
    ArmedEntity(room, id, pos),
    _score(0),
    _client(client)
{
    Stream out;
    out.setDataUChar(13);
    out.setDataChar(static_cast<u_char>(_id));
    _room.sendToAll(out);
    sendPos();
}

Player::~Player()
{
    //TODO: send to all that player is dead
}

void Player::refresh()
{
    refreshMissiles();
}

void Player::move(short dx, short dy)
{
    if (dx == 0 && dy == 0)
        return;
    if (_pos.first + dx < 0 || _pos.first + dx > SCREEN_WIDTH - PLAYER_WIDTH || _pos.second + dy < 0 || _pos.second + dy > SCREEN_HEIGHT - PLAYER_HEIGHT)
        return;
    AEntity::move(dx, dy);
}

void Player::sendPos()
{
    Stream out;
    out.setDataUChar(3);
    out.setDataChar(static_cast<u_char>(_id));
    out.setDataShort(_pos.first);
    out.setDataShort(_pos.second);
    _room.sendToAll(out);
}

void Player::fireMissile()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= FIRE_TIME) {
        ArmedEntity::fireMissile(Missile::Type::ALLY);
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
    Stream out;
    out.setDataUChar(6);
    out.setDataInt(_score);
    _room.sendToAll(out);
}

std::shared_ptr<Client> Player::client() const
{
    return _client;
}
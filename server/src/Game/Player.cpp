#include "Player.hpp"

Player::Player(Room &room, Notifier sendToAll, std::shared_ptr<Client> client, u_char id, int missileRange):
    _room(room),
    _sendToAll(sendToAll),
    _client(client),
    _pos(0, 0),
    _id(id),
    _missileRange(missileRange),
    _score(0)
{}

void Player::fireMissile()
{
    _missiles.push_back(_pos);
    (_room.*_sendToAll)("Missile fired");
}

void Player::refreshMissiles()
{
    for (auto i = _missiles.begin(); i != _missiles.end(); i++) {
        if (i->second > _missileRange) {
            _missiles.erase(i);
            (_room.*_sendToAll)("Missile destroyed");
            i--;
        } else {
            i->second++;
            // (_room.*_sendToAll)("Missile pos: " + std::to_string(i->first) + " " + std::to_string(i->second));
        }
    }
}

void Player::move(float dx, float dy)
{
    _pos.first += dx;
    _pos.second += dy;
    // (_room.*_sendToAll)("Player moved: " + std::to_string(_pos.first) + " " + std::to_string(_pos.second));
}

std::shared_ptr<Client> Player::client() const
{
    return _client;
}

const std::pair<float, float> &Player::position() const
{
    return _pos;
}

const std::vector<std::pair<float, float>> &Player::missiles() const
{
    return _missiles;
}

u_char Player::id() const
{
    return _id;
}

int Player::missileRange() const
{
    return _missileRange;
}

int Player::score() const
{
    return _score;
}

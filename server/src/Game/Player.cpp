#include "Player.hpp"
#include "../Utils/Scheduling.hpp"

Player::Player(Room &room, Notifier sendToAll, std::shared_ptr<Client> client, u_char id):
    _room(room),
    _sendToAll(sendToAll),
    _client(client),
    _pos(0, 0),
    _id(id),
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
        if (i->second > SCREEN_WIDTH) {
            _missiles.erase(i);
            (_room.*_sendToAll)("Missile destroyed");
        } else {
            i->second += MISSILE_PROGRESS_STEP;
            (_room.*_sendToAll)("Missile new pos");
        }
    }
}

void Player::move(int dx, int dy)
{
    _pos.first += dx;
    _pos.second += dy;
}

std::shared_ptr<Client> Player::client() const
{
    return _client;
}

const std::pair<int, int> &Player::position() const
{
    return _pos;
}

const std::vector<std::pair<int, int>> &Player::missiles() const
{
    return _missiles;
}

u_char Player::id() const
{
    return _id;
}

int Player::score() const
{
    return _score;
}

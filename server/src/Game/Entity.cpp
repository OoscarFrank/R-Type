#include "Entity.hpp"
#include "Room.hpp"

namespace Entities {
    // std::unique_ptr<IEntity> IEntity::create(IEntity::Type type, Room &room, int id, short x, short y)
    // {
    //     switch (type) {
    //         case IEntity::Type::PLAYER:
    //             return std::make_unique<Player>(room, id, x, y);
    //         case IEntity::Type::LITTLE_MONSTER:
    //             return std::make_unique<LittleMonster>(room, id, x, y);
    //         case IEntity::Type::MISSILE:
    //             return std::make_unique<Missile>(room, id, x, y);
    //         default:
    //             return nullptr;
    //     }
    // }

    // ---------------------------------------------------------------

    AEntity::AEntity(Room &room, int id, short x, short y):
        _room(room),
        _id(id),
        _pos(x, y),
        _lastMove(std::chrono::system_clock::now())
    {}

    AEntity::AEntity(Room &room, int id, const std::pair<short, short> &pos):
        _room(room),
        _id(id),
        _pos(pos),
        _lastMove(std::chrono::system_clock::now())
    {}

    const std::pair<short, short> &AEntity::position() const
    {
        return _pos;
    }

    int AEntity::id() const
    {
        return _id;
    }

    bool AEntity::isOutOfScreen() const
    {
        return _pos.first < 0 || _pos.first > SCREEN_WIDTH || _pos.second < 0 || _pos.second > SCREEN_HEIGHT;
    }

    void AEntity::move(short dx, short dy)
    {
        _pos.first += dx;
        _pos.second += dy;
    }

    // ---------------------------------------------------------------

    Missile::Missile(Room &room, Missile::Type type, int id, short x, short y):
        AEntity(room, id, x, y),
        _type(type)
    {}

    Missile::Missile(Room &room, Missile::Type type, int id, const std::pair<short, short> &pos):
        AEntity(room, id, pos),
        _type(type)
    {}

    Missile::~Missile()
    {
        Stream out;
        out.setDataUChar(15);
        out.setDataInt(_id);
        out.setDataUChar(_type);
        out.setDataShort(_pos.first);
        out.setDataShort(_pos.second);
        _room.sendToAll(out);
    }

    void Missile::refresh()
    {
        auto now = std::chrono::system_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= 5) { //TODO mettre un define pour le temps de déplacement
            move(_type == Missile::Type::ALLY ? ALLY_MISSILE_PROGRESS_STEP : -ENEMY_MISSILE_PROGRESS_STEP, 0);
            Stream out;
            out.setDataUChar(4);
            out.setDataInt(_id);
            out.setDataUChar(_type);
            out.setDataShort(_pos.first);
            out.setDataShort(_pos.second);
            _room.sendToAll(out);
            _lastMove = now;
        }
    }

    // ---------------------------------------------------------------

    ArmedEntity::ArmedEntity(Room &room, int id, short x, short y):
        AEntity(room, id, x, y),
        _lastFire(std::chrono::system_clock::now())
    {}

    ArmedEntity::ArmedEntity(Room &room, int id, const std::pair<short, short> &pos):
        AEntity(room, id, pos),
        _lastFire(std::chrono::system_clock::now())
    {}

    void ArmedEntity::refreshMissiles()
    {
        for (auto i = _missiles.begin(); i != _missiles.end();) {
            (**i).refresh();
            if ((**i).isOutOfScreen())
                i = _missiles.erase(i);
            else
                i++;
        }
    }

    void ArmedEntity::fireMissile(Missile::Type type)
    {
        auto now = std::chrono::system_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= FIRE_TIME) {
            _missiles.push_back(std::make_unique<Missile>(_room, type, ++_room.getMissilesIds(), _pos.first + PLAYER_WIDTH, _pos.second + PLAYER_HEIGHT / 2));
            _lastFire = now;
        }
    }

    // ---------------------------------------------------------------

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

    // ---------------------------------------------------------------

    LittleMonster::LittleMonster(Room &room, int id, short x, short y):
        ArmedEntity(room, id, x, y)
    {}

    LittleMonster::LittleMonster(Room &room, int id, const std::pair<short, short> &pos):
        ArmedEntity(room, id, pos)
    {}

    void LittleMonster::refresh()
    {
        auto now = std::chrono::system_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 1000) { //TODO mettre un define pour le temps de tir
            fireMissile(Missile::Type::ENEMY);
            _lastFire = now;
        }
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= 2) { //TODO mettre un define pour le temps de déplacement
            move(-1, 0);
            Stream out;
            out.setDataChar(7);
            out.setDataChar(static_cast<u_char>(_id));
            out.setDataShort(_pos.first);
            out.setDataShort(_pos.second);
            _room.sendToAll(out);
            _lastMove = now;
        }
        refreshMissiles();
    }
}

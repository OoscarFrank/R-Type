#include "Levels.hpp"
#include "Room.hpp"

Levels::Levels(Room &room):
    _room(room),
    _currentLvl(1)
{}

Levels::~Levels()
{}

void Levels::start()
{
    _lvlStart = chronoNow;
    _spawnLittleMonster = chronoNow;
}

void Levels::update()
{
    switch (_currentLvl) {
        case 1:
            _updateLvl1();
            break;
        case 2:
            _updateLvl2();
            break;
        case 3:
            _updateLvl3();
            break;
        case 4:
            _updateLvl4();
            break;
        case 5:
            _updateLvl5();
            break;
        case 6:
            _updateLvl6();
            break;
        case 7:
            _updateLvl7();
            break;
        case 8:
            _updateLvl8();
            break;
        case 9:
            _updateLvl9();
            break;
        case 10:
            _updateLvl10();
            break;
        default:
            break;
    }
}

size_t Levels::lvl() const
{
    return _currentLvl;
}

void Levels::_increaseLvl()
{
    std::cout << "Lvl " << ++_currentLvl << std::endl;
    _lvlStart = chronoNow;
}

void Levels::_updateLvl1()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 2000) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl2()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 1500) {
        _room.addMonster(IEntity::Type::ZIGZAGER_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl3()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 1000) {
        _room.addMonster(IEntity::Type::FOLLOWER_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl4()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 500) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl5()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 400) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl6()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 300) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl7()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 200) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl8()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 100) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl9()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 50) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
    if (chronoDiff(chronoSec, now, _lvlStart) >= 10)
        _increaseLvl();
}

void Levels::_updateLvl10()
{
    chronoTime now = chronoNow;

    if (chronoDiff(chronoMs, now, _spawnLittleMonster) >= 25) {
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
        _spawnLittleMonster = now;
    }
}

#pragma once

#include <iostream>
#include <chrono>

class Room;

using namespace std::chrono_literals;
using chronoTime = std::chrono::system_clock::time_point;
#define chronoNow std::chrono::system_clock::now()
#define chronoDiff(unit, a, b) std::chrono::duration_cast<unit>(a - b).count()
using chronoUs = std::chrono::microseconds;
using chronoMs = std::chrono::milliseconds;
using chronoSec = std::chrono::seconds;
using chronoMin = std::chrono::minutes;

class Levels
{
public:
    Levels(Room &room);
    ~Levels();

    void start();
    void update();

private:
    void _increaseLvl();

    void _updateLvl1();
    void _updateLvl2();
    void _updateLvl3();
    void _updateLvl4();
    void _updateLvl5();
    void _updateLvl6();
    void _updateLvl7();
    void _updateLvl8();
    void _updateLvl9();
    void _updateLvl10();

    Room &_room;
    size_t _currentLvl;
    chronoTime _lvlStart;
    chronoTime _spawnLittleMonster;
};

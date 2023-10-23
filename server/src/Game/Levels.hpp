#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <map>

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
    


    class Level {
        public:
            class EntityEvents
            {
                private:
                    std::map<size_t, std::vector<size_t>> _spawns;
                    unsigned char _entity;
                public:
                    EntityEvents(unsigned char entity);
                    ~EntityEvents();
                    std::vector<size_t> getSpawns(size_t lastTimecode, size_t currentTimecode) const;
                    void addSpawn(size_t timecode, size_t pos);
                    unsigned char getEntity() const;
            };

            enum Songs {
                SOUND_OF_SPACE = 1
            };

            Level(const std::string &path);
            ~Level();
            std::vector<EntityEvents> getEvents() const;
            size_t getDuration() const;
            unsigned char getStage() const;
            unsigned char getSong() const;

            class OpenError : public std::exception {
                public:
                    std::string _msg;
                    const char *what() const noexcept override { return _msg.c_str(); }
            };
            class ParsError : public std::exception {
                public:
                    std::string _msg;
                    const char *what() const noexcept override { return _msg.c_str(); }
            };
        private:
            void parsStage(const std::string &line, const std::string &path);
            void parsSong(const std::string &line, const std::string &path);
            void parsDuration(const std::string &line, const std::string &path);
            void parsEvents(const std::string &line, const std::string &path);

            std::vector<EntityEvents> _events;
            unsigned char _stage = 0;
            unsigned char _song = 0;
            size_t _duration = 0;

            char _parserEntity = -1;
    };
    Levels(Room &room);
    ~Levels();

    void start();
    void update();
    size_t lvl() const { return 1;}
    const Level &getLevel() const;

private:
    Room &_room;
    size_t _currentLvl;
    chronoTime _lastRefresh;
    chronoTime _lvlStart;
    std::vector<Levels::Level> _levels;

};
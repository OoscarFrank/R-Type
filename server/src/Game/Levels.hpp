#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <tuple>

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
                    std::vector<std::pair<size_t, std::vector<size_t>>> _spawns;
                    unsigned char _entity;
                    std::vector<std::pair<size_t, std::vector<size_t>>>::iterator _it;
                    bool _init = true;
                public:
                    /**
                     * @brief Construct a new Entity Events object
                     * 
                     * @param entity The entity to spawn
                     */
                    EntityEvents(unsigned char entity);

                    /**
                     * @brief Destroy the Entity Events object
                     * 
                     */
                    ~EntityEvents();

                    /**
                     * @brief Get all entities that should spawn at the current timecode
                     * 
                     * @param currentTimecode The current timecode
                     * @return std::vector<size_t> The entities to spawn
                     */
                    std::vector<size_t> getSpawns(size_t currentTimecode);

                    /**
                     * @brief Add an entity to spawn
                     * 
                     * @param timecode The timecode to spawn the entity
                     * @param pos The position of the entity
                     */
                    void addSpawn(size_t timecode, size_t pos);

                    /**
                     * @brief Get the entity type
                     * 
                     * @return The entity type
                     */
                    unsigned char getEntity() const;

                    /**
                     * @brief Sort the spawns queue
                     * 
                     */
                    void sort();

                    /**
                     * @brief Check if the entity queue is finished spawning
                     * 
                     * @return true If the entity queue is finished spawning
                     * @return false If the entity  queue is not finished spawning
                     */
                    bool isFinished() const;

                    /**
                     * @brief Set the init value
                     * 
                     * @param init The new init value
                     */
                    void setInit(bool init);
            };

            class StrobeEvent
            {
                private:
                    std::vector<std::tuple<size_t, unsigned char, bool>> _strobe;
                    std::vector<std::tuple<size_t, unsigned char, bool>>::iterator _it;
                    bool _init = true;
                public:
                    /**
                     * @brief Construct a new Strobe Event object
                     * 
                     */
                    StrobeEvent();

                    /**
                     * @brief Destroy the Strobe Event object
                     * 
                     */
                    ~StrobeEvent();

                    /**
                     * @brief Get all strobes that should happen at the current timecode
                     * 
                     * @param currentTimecode The current timecode
                     * @return std::vector<std::tuple<size_t, unsigned char, bool>> The strobes to happen
                     */
                    std::vector<std::tuple<size_t, unsigned char, bool>> getEvents(size_t currentTimecode);

                    /**
                     * @brief Add a strobe
                     * 
                     * @param timecode The timecode to strobe
                     * @param color The color to strobe
                     * @param duration The duration of the strobe
                     */
                    void addColor(size_t timecode, unsigned char color, size_t duration);

                    /**
                     * @brief Sort the strobes queue
                     * 
                     */
                    void sort();

                    /**
                     * @brief Check if the strobes queue is finished
                     * 
                     * @return true If the strobes queue is finished
                     * @return false If the strobes queue is not finished
                     */
                    bool isFinished() const;

                    /**
                     * @brief Set the init value
                     * 
                     * @param init The new init value
                     */
                    void setInit(bool init);

                    enum {
                        RED = 1,
                        GREEN,
                        BLUE,
                        YELLOW,
                        PURPLE,
                        CYAN,
                        WHITE
                    };
            };

            enum Songs {
                SOUND_OF_SPACE = 1,
                TURN_ON_THE_LIGHTS,
                PUSH_UP,
                VOIS_SUR_TON_CHEMIN,
                HEUTE_NACHT,
                CLEON,
                AMNESIA,
                SEVENNATION,
                BLAHBLAH
            };

            /**
             * @brief Construct a new Level object
             * 
             * @param path The path to the level file
             */
            Level(const std::string &path);

            /**
             * @brief Destroy the Level object
             * 
             */
            ~Level();

            /**
             * @brief Get the events
             * 
             * @return std::vector<EntityEvents>& The events
             */
            std::vector<EntityEvents> &getEvents();

            /**
             * @brief Get the strobes
             * 
             * @return StrobeEvent& The strobes
             */
            StrobeEvent &getStrobes();

            /**
             * @brief Get the song
             * 
             * @return unsigned char The song
             */
            unsigned char getSong() const;

            /**
             * @brief Check if the level is ended
             * 
             * @return true If the level is ended
             * @return false If the level is not ended
             */
            bool isEnded() const;

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
            void parsSong(const std::string &line, const std::string &path, size_t line_nb);
            void parsEvents(const std::string &line, const std::string &path, size_t line_nb);
            std::vector<EntityEvents> _events;
            StrobeEvent _strobes;
            unsigned char _song = 0;

            char _parserEntity = -1;
    };

    /**
     * @brief Construct a new Levels object
     * 
     * @param files The files to load
     */
    Levels(std::vector<std::string> files);

    /**
     * @brief Destroy the Levels object
     * 
     */
    ~Levels();

    /**
     * @brief start the game
     * 
     */
    void start();

    /**
     * @brief update the current level
     * 
     * @param room 
     */
    void update(Room &room);

    /**
     * @brief Get the current level
     * 
     * @return const Level& The current level
     */
    const Level &getLevel() const;

private:
    size_t _currentLvl;
    chronoTime _lvlStart;
    chronoTime _endTime;
    chronoTime _lastUpdate;
    bool _ended = false;
    std::vector<Levels::Level> _levels;

};
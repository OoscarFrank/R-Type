#include "Levels.hpp"
#include "Room.hpp"

Levels::Levels(Room &room):
    _room(room),
    _currentLvl(0)
{
    try {
        this->_levels.push_back(Levels::Level("stages/stage1.script"));
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

Levels::~Levels()
{}

void Levels::start()
{
    _lvlStart = chronoNow;
    _lastRefresh = chronoNow;
}

const Levels::Level &Levels::getLevel() const
{
    return this->_levels[_currentLvl];
}

void Levels::update()
{
    size_t last = chronoDiff(chronoMs, _lastRefresh, _lvlStart);
    this->_lastRefresh = chronoNow;
    size_t current = chronoDiff(chronoMs, chronoNow, _lvlStart);
    std::vector<size_t> tmp;

    tmp = this->_levels[_currentLvl].getEvents()[Monster::LITTLE_MONSTER - 2].getSpawns(last, current);
    for (auto i = tmp.begin(); i != tmp.end(); ++i)
        _room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, (*i));

    tmp = this->_levels[_currentLvl].getEvents()[Monster::ZIGZAGER_MONSTER - 2].getSpawns(last, current);
    for (auto i = tmp.begin(); i != tmp.end(); ++i)
        _room.addMonster(IEntity::Type::ZIGZAGER_MONSTER, SCREEN_WIDTH, (*i));

    tmp = this->_levels[_currentLvl].getEvents()[Monster::FOLLOWER_MONSTER - 2].getSpawns(last, current);
    for (auto i = tmp.begin(); i != tmp.end(); ++i)
        _room.addMonster(IEntity::Type::FOLLOWER_MONSTER, SCREEN_WIDTH, (*i));
}

Levels::Level::EntityEvents::EntityEvents(unsigned char entity)
{
    this->_entity = entity;
}

Levels::Level::EntityEvents::~EntityEvents()
{

}

std::vector<size_t> Levels::Level::EntityEvents::getSpawns(size_t lastTimecode, size_t currentTimecode) const
{
    std::vector<size_t> out;
    for(auto i = _spawns.begin(); i != _spawns.end(); ++i) {
        if (i->first > lastTimecode && i->first <= currentTimecode) {
            for(auto j = i->second.begin(); j != i->second.end(); ++j) {
                out.push_back(*j);
            }
        }
    }
    return out;
}

void Levels::Level::EntityEvents::addSpawn(size_t timecode, size_t pos)
{
    if (this->_spawns[timecode].empty()) {
        this->_spawns[timecode] = std::vector<size_t>();
    }
    this->_spawns[timecode].push_back(pos);
}

unsigned char Levels::Level::EntityEvents::getEntity() const
{
    return this->_entity;
}

Levels::Level::Level(const std::string &path)
{
    std::ifstream file(path);

    if (!file.is_open()) {
        Levels::Level::OpenError err;
        err._msg =  "Erreur lors de l'ouverture du fichier " + path;
        throw err;
        return;
    }

    this->_events.push_back(EntityEvents(Monster::LITTLE_MONSTER));
    this->_events.push_back(EntityEvents(Monster::ZIGZAGER_MONSTER));
    this->_events.push_back(EntityEvents(Monster::FOLLOWER_MONSTER));

    std::string line;
    while (getline(file, line)) {
        parsStage(line, path);
        parsSong(line, path);
        parsDuration(line, path);
        parsEvents(line, path);
    }
    file.close();
}

Levels::Level::~Level()
{
}

void Levels::Level::parsStage(const std::string &line, const std::string &path)
{
    if (line.find("STAGE") != std::string::npos) {
        size_t pos;
        if ((pos = line.find(":")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Erreur lors de la lecture du fichier " + path + " : STAGE";
            throw err;
            return;
        } else {
            try {
                this->_stage = std::stoi(line.substr(pos + 1));
            } catch (const std::exception& e) {
                Levels::Level::ParsError err;
                err._msg =  "Erreur lors de la lecture du fichier " + path + " : STAGE";
                throw err;
                return;
            }
        }
    }
}

void Levels::Level::parsDuration(const std::string &line, const std::string &path)
{
    if (line.find("DURATION") != std::string::npos) {
        size_t pos;
        if ((pos = line.find(":")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Erreur lors de la lecture du fichier " + path + " : DURATION";
            throw err;
            return;
        } else {
            try {
                this->_duration = std::stoul(line.substr(pos + 1));
            } catch (const std::exception& e) {
                Levels::Level::ParsError err;
                err._msg =  "Erreur lors de la lecture du fichier " + path + " : DURATION";
                throw err;
                return;
            }
        }
    }
}

void Levels::Level::parsSong(const std::string &line, const std::string &path)
{
    if (line.find("SONG") != std::string::npos) {
        size_t pos;
        if ((pos = line.find(":")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Erreur lors de la lecture du fichier " + path + " : SONG";
            throw err;
            return;
        } else {
            std::string song = line.substr(pos + 1);
            if (song.find("SOUND_OF_SPACE") != std::string::npos)
                this->_song = Levels::Level::SOUND_OF_SPACE;
            else {
                Levels::Level::ParsError err;
                err._msg =  "Erreur lors de la lecture du fichier " + path + " : SONG";
                throw err;
                return;
            }
        }
    }
}

void Levels::Level::parsEvents(const std::string &line, const std::string &path)
{

    if (line.find("LITTLE_MONSTER") != std::string::npos)
        this->_parserEntity = Monster::LITTLE_MONSTER;
    else if (line.find("FOLLOWER_MONSTER") != std::string::npos)
        this->_parserEntity = Monster::FOLLOWER_MONSTER;
    else if (line.find("ZIGZAGER_MONSTER") != std::string::npos)
        this->_parserEntity = Monster::ZIGZAGER_MONSTER;
 
    if (this->_parserEntity != -1) {
        size_t timeCode = 0;
        size_t pos;
        size_t pipePos;
        std::vector<size_t> poses;
        std::string posesStr;
        try {
            timeCode = std::stoul(line);
        } catch (const std::exception& e) {
            return;
        }
        if ((pos = line.find(":")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Erreur lors de la lecture du fichier " + path + " : EVENT";
            throw err;
            return;
        }
        if ((pipePos = line.find("|", pos + 1)) == std::string::npos) 
            pipePos = line.size();
        posesStr = line.substr(pos + 1, pipePos);
        std::stringstream stream(posesStr);
        std::string tmp;
        while (std::getline(stream, tmp, ',')) {
            try {
                size_t value = std::stol(tmp);
                poses.push_back(value);
            } catch (const std::exception& e) {
                Levels::Level::ParsError err;
                err._msg =  "Erreur lors de la lecture du fichier " + path + " : EVENT";
                throw err;
                return;
            }
        }
        if (pipePos == line.size()) {
            for(auto i = poses.begin(); i != poses.end(); ++i) {
                this->_events[this->_parserEntity - 2 /* +2 parcque enum monstres commence a 2 */] .addSpawn(timeCode, (*i));
            }
        } else {
            std::string newLine = line.substr(pipePos + 1);
            size_t freq = 0;
            size_t times = 0;
            if ((pipePos = newLine.find("|")) == std::string::npos) {
                Levels::Level::ParsError err;
                err._msg =  "Erreur lors de la lecture du fichier " + path + " : EVENT";
                throw err;
                return;
            }
            try {
                freq = std::stoul(newLine);
                times = std::stoul(newLine.substr(pipePos + 1));
            } catch (const std::exception& e) {
                Levels::Level::ParsError err;
                err._msg =  "Erreur lors de la lecture du fichier " + path + " : EVENT";
                throw err;
                return;
            }
            size_t it = 0;
            for (size_t i = 0; i < times + 1; i++) {
                this->_events[this->_parserEntity - 2 /* +2 parcque enum monstres commence a 2 */] .addSpawn(timeCode + (freq * i), poses[it]);
                ++it;
                if (it == poses.size())
                    it = 0;
            }
        }
    }
}


std::vector<Levels::Level::EntityEvents> Levels::Level::getEvents() const
{
    return this->_events;
}

size_t Levels::Level::getDuration() const
{
    return this->_duration;
}

unsigned char Levels::Level::getStage() const
{
    return this->_stage;
}

unsigned char Levels::Level::getSong() const
{
    return this->_song;
}

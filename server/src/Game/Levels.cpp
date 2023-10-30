#include "Levels.hpp"
#include "Room.hpp"
#include <algorithm>
#include <string>


Levels::Levels(std::vector<std::string> files)
{
    _currentLvl = 0;
    try {
        for(auto i = files.begin(); i != files.end(); ++i)
            this->_levels.push_back(Levels::Level((*i)));
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
}

const Levels::Level &Levels::getLevel() const
{
    return this->_levels[_currentLvl];
}

void Levels::update(Room &room)
{
    if (!_ended) {

        size_t current = chronoDiff(chronoMs, chronoNow, _lvlStart);
        std::vector<size_t> tmp;

        tmp = this->_levels[_currentLvl].getEvents()[Monster::LITTLE_MONSTER - 2].getSpawns(current);
        for (auto i = tmp.begin(); i != tmp.end(); ++i)
            room.addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, (*i));

        tmp = this->_levels[_currentLvl].getEvents()[Monster::ZIGZAGER_MONSTER - 2].getSpawns(current);
        for (auto i = tmp.begin(); i != tmp.end(); ++i)
            room.addMonster(IEntity::Type::ZIGZAGER_MONSTER, SCREEN_WIDTH, (*i));

        tmp = this->_levels[_currentLvl].getEvents()[Monster::FOLLOWER_MONSTER - 2].getSpawns(current);
        for (auto i = tmp.begin(); i != tmp.end(); ++i)
            room.addMonster(IEntity::Type::FOLLOWER_MONSTER, SCREEN_WIDTH, (*i));

        tmp = this->_levels[_currentLvl].getEvents()[Monster::BURST_MONSTER - 2].getSpawns(current);
        for (auto i = tmp.begin(); i != tmp.end(); ++i)
            room.addMonster(IEntity::Type::BURST_MONSTER, SCREEN_WIDTH, (*i));

        tmp = this->_levels[_currentLvl].getEvents()[Monster::BOSS1 - 2].getSpawns(current);
        for (auto i = tmp.begin(); i != tmp.end(); ++i)
            room.addMonster(IEntity::Type::BOSS1, SCREEN_WIDTH, (*i));

        tmp = this->_levels[_currentLvl].getEvents()[Monster::BOSS2 - 2].getSpawns(current);
        for (auto i = tmp.begin(); i != tmp.end(); ++i)
            room.addMonster(IEntity::Type::BOSS2, SCREEN_WIDTH, (*i));

        std::vector<std::tuple<size_t, unsigned char, bool>> strobes = this->_levels[_currentLvl].getStrobes().getEvents(current);
        for(auto i = strobes.begin(); i != strobes.end(); ++i) {
            room.sendToAll(StreamFactory::strobe(std::get<1>(*i), std::get<2>(*i)));
        }
        if (this->_levels[_currentLvl].isEnded() && !room.isMonster()) {
            _ended = true;
            _endTime = chronoNow;
            _currentLvl += 1;
            _lastUpdate = chronoNow;
        }
    }
    if (_ended) {
        if (_currentLvl < this->_levels.size()) {
            if (chronoDiff(chronoMs, chronoNow, _endTime) < TIMEOUT_BETWEEN_LEVELS ) {
                if (chronoDiff(chronoMs, chronoNow, _lastUpdate) > 200) {
                    room.sendToAll(StreamFactory::changeLevel(chronoDiff(chronoMs, chronoNow, _endTime), this->_levels[_currentLvl].getSong(), false));
                    _lastUpdate = chronoNow;
                }
            } else {
                room.sendToAll(StreamFactory::changeLevel(0, this->_levels[_currentLvl].getSong(), true));
                _ended = false;
                _lvlStart = chronoNow;
            }
        } else {
            std::cout << "NO MORE STAGES !!!" << std::endl;
        }
    }


}





Levels::Level::EntityEvents::EntityEvents(unsigned char entity)
{
    this->_entity = entity;
}

Levels::Level::EntityEvents::~EntityEvents()
{

}

std::vector<size_t> Levels::Level::EntityEvents::getSpawns(size_t currentTimecode)
{
    std::vector<size_t> out;

    if (_init) {
        _it = _spawns.begin();
        _init = false;
    }
    for(; _it != _spawns.end(); ++_it) {
        if ( _it->first <= currentTimecode) {
            for(auto j = _it->second.begin(); j != _it->second.end(); ++j) {
                out.push_back(*j);
            }
        } else {
            break;
        }
    }
    return out;
}

void Levels::Level::EntityEvents::addSpawn(size_t timecode, size_t pos)
{
    bool found = false;
    for(auto i = this->_spawns.begin(); i != this->_spawns.end(); ++i) {
        if ((*i).first == timecode) {
            (*i).second.push_back(pos);
            return;
        }
    }
    this->_spawns.push_back(std::make_pair(timecode, std::vector<size_t>{pos}));
}

unsigned char Levels::Level::EntityEvents::getEntity() const
{
    return this->_entity;
}

void Levels::Level::EntityEvents::sort()
{
    std::sort(_spawns.begin(), _spawns.end(), [](std::pair<size_t, std::vector<size_t>> a, std::pair<size_t, std::vector<size_t>> b) {
        return a.first < b.first;
    });
}


bool Levels::Level::EntityEvents::isFinished() const
{
    return _it == _spawns.end();
}

void Levels::Level::EntityEvents::setInit(bool init)
{
    this->_init = init;
}




Levels::Level::StrobeEvent::StrobeEvent()
{

}

Levels::Level::StrobeEvent::~StrobeEvent()
{
}

std::vector<std::tuple<size_t, unsigned char, bool>> Levels::Level::StrobeEvent::getEvents(size_t currentTimecode)
{
    std::vector<std::tuple<size_t, unsigned char, bool>> out;

    if (_init) {
        _it = _strobe.begin();
        _init = false;
    }
    for(; _it != _strobe.end(); ++_it) {
        if ( std::get<0>(*_it) <= currentTimecode) {
            out.push_back(*_it);
        } else {
            break;
        }
    }
    return out;
}

void Levels::Level::StrobeEvent::addColor(size_t timecode, unsigned char color, size_t duration)
{
    std::tuple<size_t, unsigned char, bool> tmpOn(timecode, color, true);
    this->_strobe.push_back(tmpOn);
    std::tuple<size_t, unsigned char, bool> tmpOff(timecode + duration, color, false);
    this->_strobe.push_back(tmpOff);
}

void Levels::Level::StrobeEvent::sort()
{
    std::sort(_strobe.begin(), _strobe.end(), [](std::tuple<size_t, unsigned char, bool> a, std::tuple<size_t, unsigned char, bool> b) {
        return std::get<0>(a) < std::get<0>(b);
    });
}

bool Levels::Level::StrobeEvent::isFinished() const
{
    return _it == _strobe.end();
}


void Levels::Level::StrobeEvent::setInit(bool init)
{
    this->_init = init;
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
    this->_events.push_back(EntityEvents(Monster::BURST_MONSTER));
    this->_events.push_back(EntityEvents(Monster::BOSS1));
    this->_events.push_back(EntityEvents(Monster::BOSS2));


    std::string line;
    size_t line_nb = 1;
    while (getline(file, line)) {
        std::string newLine;
        for (char c : line) {
            if (c != ' ')
                newLine += c;
            
        }
        parsSong(newLine, path, line_nb);
        parsEvents(newLine, path, line_nb);
        line_nb += 1;
    }
    file.close();

    for (auto i = this->_events.begin(); i != this->_events.end(); ++i)
        (*i).sort();
}

Levels::Level::~Level()
{
}

void Levels::Level::parsSong(const std::string &line, const std::string &path, size_t line_nb)
{
    if (line.find("SONG") != std::string::npos) {
        size_t pos;
        if ((pos = line.find(":")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Error while reading file : " + path + "\nMissing ':'" + "\nLine : " + std::to_string(line_nb);
            throw err;
            return;
        } else {
            std::string song = line.substr(pos + 1);
            if (song.find("SOUND_OF_SPACE") != std::string::npos)
                this->_song = Levels::Level::SOUND_OF_SPACE;
            else if (song.find("TURN_ON_THE_LIGHTS") != std::string::npos)
                this->_song = Levels::Level::TURN_ON_THE_LIGHTS;
            else if (song.find("PUSH_UP") != std::string::npos)
                this->_song = Levels::Level::PUSH_UP;
            else if (song.find("VOIS_SUR_TON_CHEMIN") != std::string::npos)
                this->_song = Levels::Level::VOIS_SUR_TON_CHEMIN;
            else {
                Levels::Level::ParsError err;
                err._msg =  "Error while reading file : " + path + "\nInvalid song : " + song + "\nLine : " + std::to_string(line_nb) + ". Errur";
                throw err;
                return;
            }
        }
    }
}

void Levels::Level::parsEvents(const std::string &line, const std::string &path, size_t line_nb)
{

    if (line.find("STROBES") != std::string::npos)
        this->_parserEntity = 1;
    else if (line.find("LITTLE_MONSTER") != std::string::npos)
        this->_parserEntity = Monster::LITTLE_MONSTER;
    else if (line.find("FOLLOWER_MONSTER") != std::string::npos)
        this->_parserEntity = Monster::FOLLOWER_MONSTER;
    else if (line.find("ZIGZAGER_MONSTER") != std::string::npos)
        this->_parserEntity = Monster::ZIGZAGER_MONSTER;
    else if (line.find("BURST_MONSTER") != std::string::npos)
        this->_parserEntity = Monster::BURST_MONSTER;
    else if (line.find("BOSS1") != std::string::npos)
        this->_parserEntity = Monster::BOSS1;
    else if (line.find("BOSS2") != std::string::npos)
        this->_parserEntity = Monster::BOSS2;

    if (this->_parserEntity != -1 && this->_parserEntity != 1) {
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
            err._msg =  "Error while reading file : " + path + "\nMissing ':'" + "\nLine : " + std::to_string(line_nb);
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
                err._msg =  "Error while reading file : " + path + "\nInvalid monster pos : " + tmp + "\nLine : " + std::to_string(line_nb);
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
                err._msg =  "Error while reading file : " + path + "\nMissing '|'" + "\nLine : " + std::to_string(line_nb);
                throw err;
                return;
            }
            try {
                freq = std::stoul(newLine);
            } catch (const std::exception& e) {
                Levels::Level::ParsError err;
                err._msg =  "Error while reading file : " + path + "\nInvalid value :" + newLine + "\nLine : " + std::to_string(line_nb);
                throw err;
                return;
            }

            try {
                times = std::stoul(newLine.substr(pipePos + 1));
            } catch (const std::exception& e) {
                Levels::Level::ParsError err;
                err._msg =  "Error while reading file : " + path + "\nInvalid value :" + newLine.substr(pipePos + 1) + "\nLine : " + std::to_string(line_nb);
                throw err;
                return;
            }

            size_t it = 0;
            for (size_t i = 0; i < times; i++) {
                this->_events[this->_parserEntity - 2 /* +2 parcque enum monstres commence a 2 */].addSpawn(timeCode + (freq * i), poses[it]);
                ++it;
                if (it == poses.size())
                    it = 0;
            }
        }
    } else if (this->_parserEntity == 1) {
        size_t timeCode = 0;
        size_t pos;
        size_t pipePos;
        try {
            timeCode = std::stoul(line);
        } catch (const std::exception& e) {
            return;
        }
        if ((pos = line.find(":")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Error while reading file : " + path + "\nMissing ':'" + "\nLine : " + std::to_string(line_nb);
            throw err;
            return;
        }
        if ((pipePos = line.find("|")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Error while reading file : " + path + "\nMissing '|'" + "\nLine : " + std::to_string(line_nb);
            throw err;
            return;
        }
        std::string colorStr = line.substr(pos + 1, pipePos);
        unsigned char color = 0;
        if (colorStr.find("red") != std::string::npos)
            color = StrobeEvent::RED;
        else if (colorStr.find("blue") != std::string::npos)
            color = StrobeEvent::BLUE;
        else if (colorStr.find("green") != std::string::npos)
            color = StrobeEvent::GREEN;
        else if (colorStr.find("yellow") != std::string::npos)
            color = StrobeEvent::YELLOW;
        else if (colorStr.find("cyan") != std::string::npos)
            color = StrobeEvent::CYAN;
        else if (colorStr.find("purple") != std::string::npos)
            color = StrobeEvent::PURPLE;
        else if (colorStr.find("white") != std::string::npos)
            color = StrobeEvent::WHITE;
        else {
            Levels::Level::ParsError err;
            err._msg =  "Error while reading file : " + path + "\nInvalid color : " + colorStr + "\nLine : " + std::to_string(line_nb);
            throw err;
            return;
        }
        std::string newLine = line.substr(pipePos + 1);
        if ((pipePos = newLine.find("|")) == std::string::npos) {
            Levels::Level::ParsError err;
            err._msg =  "Error while reading file : " + path + "\nMissing '|'" + "\nLine : " + std::to_string(line_nb);
            throw err;
            return;
        }
        size_t duration = 0;
        try {
            duration = std::stoul(newLine);
        } catch (const std::exception& e) {
            Levels::Level::ParsError err;
            err._msg =  "Error while reading file : " + path + "\nInvalid value : " + newLine + "\nLine : " + std::to_string(line_nb);
            throw err;
            return;
        }

        newLine = newLine.substr(pipePos + 1);
        size_t times = 0;
        try {
            times = std::stoul(newLine);
        } catch (const std::exception& e) {
            Levels::Level::ParsError err;
            err._msg =  "Error while reading file : " + path + "\nInvalid value : " + newLine + "\nLine : " + std::to_string(line_nb);
            throw err;
            return;
        }

        for (size_t i = 0; i < times; i++) {

            _strobes.addColor(timeCode + (i * (duration * 2)), color, duration);
        }
    }
}


Levels::Level::StrobeEvent &Levels::Level::getStrobes()
{
    return this->_strobes;
}

std::vector<Levels::Level::EntityEvents> &Levels::Level::getEvents()
{
    return this->_events;
}

// unsigned char Levels::Level::getStage() const
// {
//     return this->_stage;
// }

unsigned char Levels::Level::getSong() const
{
    return this->_song;
}

bool Levels::Level::isEnded() const
{
    for(auto i = this->_events.begin(); i != this->_events.end(); ++i) {
        if(!(*i).isFinished()) {
            return false;
        }
    }

    if (!this->_strobes.isFinished())
        return false;
    return true;
}
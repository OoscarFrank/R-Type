#include "Stream.hpp"


Stream::Stream()
{

}

Stream::Stream(const Stream &stream)
{
    this->_buffer = stream._buffer;
}

Stream::Stream(const std::vector<unsigned char> &buffer)
{
    this->_buffer = buffer;
}

Stream::Stream(const std::string &str)
{
    for (auto &c : str)
        this->_buffer.push_back(c);
}

Stream::Stream(const char str[], size_t size)
{
    for (size_t i = 0; i < size; i++)
        this->_buffer.push_back(str[i]);
}

Stream::~Stream()
{

}

void Stream::operator += (const unsigned char &data)
{
    this->_buffer.push_back(data);
}

void Stream::operator += (const Stream &stream)
{
    for (auto &c : stream.getBuffer())
        this->_buffer.push_back(c);
}

void Stream::operator += (const std::string &str)
{
    for (auto &c : str)
        this->_buffer.push_back(c);
}

Stream Stream::operator = (const Stream &stream)
{
    this->_buffer = stream._buffer;
    return *this;
}


unsigned char Stream::operator[](size_t index) const
{
    return this->_buffer[index];
}

unsigned char &Stream::operator[](size_t index)
{
    return this->_buffer[index];
}

const std::vector<unsigned char> &Stream::getBuffer() const
{
    return this->_buffer;
}

unsigned int Stream::getDataUInt()
{
    unsigned int out = 0;

    out += this->_buffer[0] << 24;
    out += this->_buffer[1] << 16;
    out += this->_buffer[2] << 8;
    out += this->_buffer[3];

    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());

    return out;
}

unsigned short Stream::getDataUShort()
{
    unsigned short out = 0;

    out += this->_buffer[0] << 8;
    out += this->_buffer[1];

    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());

    return out;
}

unsigned char Stream::getDataUChar()
{
    unsigned char out = 0;

    out += this->_buffer[0];
    this->_buffer.erase(this->_buffer.begin());

    return out;
}


int Stream::getDataInt()
{
    int out = 0;

    out += this->_buffer[0] << 24;
    out += this->_buffer[1] << 16;
    out += this->_buffer[2] << 8;
    out += this->_buffer[3];

    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());

    return out;
}

short Stream::getDataShort()
{
    short out = 0;

    out += this->_buffer[0] << 8;
    out += this->_buffer[1];

    this->_buffer.erase(this->_buffer.begin());
    this->_buffer.erase(this->_buffer.begin());

    return out;
}

char Stream::getDataChar()
{
    char out = 0;

    out += this->_buffer[0];
    this->_buffer.erase(this->_buffer.begin());

    return out;
}


void Stream::setDataUInt(unsigned int data)
{
    unsigned char tmp = data >> 24;
    this->_buffer.push_back(tmp);
    tmp = data >> 16;
    this->_buffer.push_back(tmp);
    tmp = data >> 8;
    this->_buffer.push_back(tmp);
    tmp = data;
    this->_buffer.push_back(tmp);
}

void Stream::setDataUShort(unsigned short data)
{
    unsigned char tmp = data >> 8;
    this->_buffer.push_back(tmp);
    tmp = data;
    this->_buffer.push_back(tmp);
}

void Stream::setDataUChar(unsigned char data)
{
    this->_buffer.push_back(data);
}

void Stream::setDataInt(int data)
{
    unsigned char tmp = data >> 24;
    this->_buffer.push_back(tmp);
    tmp = data >> 16;
    this->_buffer.push_back(tmp);
    tmp = data >> 8;
    this->_buffer.push_back(tmp);
    tmp = data;
    this->_buffer.push_back(tmp);
}

void Stream::setDataShort(short data)
{
    unsigned char tmp = data >> 8;
    this->_buffer.push_back(tmp);
    tmp = data;
    this->_buffer.push_back(tmp);
}

void Stream::setDataChar(char data)
{
    this->_buffer.push_back(data);
}


void Stream::setDataCharArray(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        this->_buffer.push_back(data[i]);
}

Stream Stream::subStream(size_t pos) const
{
    Stream out;

    if (pos > this->_buffer.size()) {
        Stream::SubStreamError error;
            error._message = "'size' is out of range" + std::to_string(pos) + " > " + std::to_string(this->_buffer.size());
            throw error;
    }

    for (size_t i = pos; i < this->_buffer.size(); i++)
        out += this->_buffer[i];

    return out;
}

Stream Stream::subStream(size_t start, size_t size) const
{
    Stream out;

    if (start + size > this->_buffer.size()) {
        Stream::SubStreamError error;
            error._message = "'start + size' is out of range" + std::to_string(start + size) + " > " + std::to_string(this->_buffer.size());
            throw error;
    }

    for (size_t i = start; i < start + size; i++)
        out += this->_buffer[i];

    return out;
}

size_t Stream::size() const
{
    return this->_buffer.size();
}

void Stream::clear()
{
    this->_buffer.clear();
}

std::string Stream::toString() const
{
    std::string out;

    for (auto &c : this->_buffer)
        out += c;

    return out;
}

std::ostream &operator<<(std::ostream &os,const Stream &stream) {
    os << "[";

    const std::vector<unsigned char> &data = stream.getBuffer();
    for (auto i = data.begin(); i != data.end(); i++) {
        os << std::to_string((*i));
        if (i + 1 != data.end())
            os << ", ";
    }

    return os << "]";
}

Stream StreamFactory::screenProgress(u_int progress)
{
    Stream out;
    out.setDataUChar(1);
    out.setDataUInt(progress);
    return out;
}

Stream StreamFactory::playerPos(u_int id, short x, short y)
{
    Stream out;
    out.setDataUChar(3);
    out.setDataUInt(id);
    out.setDataShort(x);
    out.setDataShort(y);
    return out;
}

Stream StreamFactory::missilePos(u_int id, u_char type, short x, short y)
{
    Stream out;
    out.setDataUChar(4);
    out.setDataUInt(id);
    out.setDataUChar(type);
    out.setDataShort(x);
    out.setDataShort(y);
    return out;
}

Stream StreamFactory::score(int score)
{
    Stream out;
    out.setDataUChar(6);
    out.setDataInt(score);
    return out;
}

Stream StreamFactory::monsterPos(u_int id, u_char type, short x, short y)
{
    Stream out;
    out.setDataUChar(7);
    out.setDataUInt(id);
    out.setDataUChar(type);
    out.setDataShort(x);
    out.setDataShort(y);
    return out;
}

Stream StreamFactory::joinRoom(u_int roomId, u_int playerId)
{
    Stream out;
    out.setDataUChar(10);
    out.setDataUInt(roomId);
    out.setDataUInt(playerId);
    return out;
}

Stream StreamFactory::waitGame(int time, bool start, unsigned char song)
{
    Stream out;
    out.setDataUChar(11);
    out.setDataInt(time);
    out.setDataUChar(start);
    out.setDataUChar(song);
    return out;
}

Stream StreamFactory::playerJoinedGame(u_int playerId)
{
    Stream out;
    out.setDataUChar(13);
    out.setDataUInt(playerId);
    return out;
}

Stream StreamFactory::playerLeftGame(u_int playerId)
{
    Stream out;
    out.setDataUChar(14);
    out.setDataUInt(playerId);
    return out;
}

Stream StreamFactory::missileDestroyed(u_int id, u_char type, short x, short y)
{
    Stream out;
    out.setDataUChar(15);
    out.setDataUInt(id);
    out.setDataUChar(type);
    out.setDataShort(x);
    out.setDataShort(y);
    return out;
}

Stream StreamFactory::monsterDied(u_int id)
{
    Stream out;
    out.setDataUChar(16);
    out.setDataUInt(id);
    return out;
}

Stream StreamFactory::gameOver(u_char type)
{
    Stream out;
    out.setDataUChar(17);
    out.setDataUChar(type);
    return out;
}

Stream StreamFactory::playerDied(u_int id)
{
    Stream out;
    out.setDataUChar(18);
    out.setDataUInt(id);
    return out;
}

Stream StreamFactory::playerLife(int life)
{
    Stream out;
    out.setDataUChar(19);
    out.setDataInt(life);
    return out;
}

Stream StreamFactory::monsterLife(u_int id, int life)
{
    Stream out;
    out.setDataUChar(20);
    out.setDataUInt(id);
    out.setDataInt(life);
    return out;
}

Stream StreamFactory::askResend(u_short nbr)
{
    Stream out;
    out.setDataUChar(255);
    out.setDataUShort(nbr);
    return out;
}

Stream StreamFactory::strobe(u_char color, u_int duration, u_int times)
{
    Stream out;
    out.setDataUChar(21);
    out.setDataUChar(color);
    out.setDataUInt(duration);
    out.setDataUInt(times);
    return out;
}
#include "Stream.hpp"

using namespace TypesLitterals;

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

unsigned long Stream::getDataULong()
{
    unsigned long out = 0;

    out += this->_buffer[0];
    out <<= 8;
    out += this->_buffer[1];
    out <<= 8;
    out += this->_buffer[2];
    out <<= 8;
    out += this->_buffer[3];
    out <<= 8;
    out += this->_buffer[4];
    out <<= 8;
    out += this->_buffer[5];
    out <<= 8;
    out += this->_buffer[6];
    out <<= 8;
    out += this->_buffer[7];

    for (int i = 0; i < 8; i++)
        this->_buffer.erase(this->_buffer.begin());

    return out;
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

long Stream::getDataLong()
{
    long out = 0;

    out += this->_buffer[0];
    out <<= 8;
    out += this->_buffer[1];
    out <<= 8;
    out += this->_buffer[2];
    out <<= 8;
    out += this->_buffer[3];
    out <<= 8;
    out += this->_buffer[4];
    out <<= 8;
    out += this->_buffer[5];
    out <<= 8;
    out += this->_buffer[6];
    out <<= 8;
    out += this->_buffer[7];

    for (int i = 0; i < 8; i++)
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

Stream &Stream::operator>>(u_char &data)
{
    data = this->getDataUChar();
    return *this;
}

Stream &Stream::operator>>(u_short &data)
{
    data = this->getDataUShort();
    return *this;
}

Stream &Stream::operator>>(u_int &data)
{
    data = this->getDataUInt();
    return *this;
}

Stream &Stream::operator>>(u_long &data)
{
    data = this->getDataULong();
    return *this;
}

Stream &Stream::operator>>(char &data)
{
    data = this->getDataChar();
    return *this;
}

Stream &Stream::operator>>(short &data)
{
    data = this->getDataShort();
    return *this;
}

Stream &Stream::operator>>(int &data)
{
    data = this->getDataInt();
    return *this;
}

Stream &Stream::operator>>(long &data)
{
    data = this->getDataLong();
    return *this;
}

Stream &Stream::operator>>(bool &data)
{
    data = this->getDataUChar();
    return *this;
}

void Stream::setDataULong(unsigned long data)
{
    unsigned char tmp = data >> 56;
    this->_buffer.push_back(tmp);
    tmp = data >> 48;
    this->_buffer.push_back(tmp);
    tmp = data >> 40;
    this->_buffer.push_back(tmp);
    tmp = data >> 32;
    this->_buffer.push_back(tmp);
    tmp = data >> 24;
    this->_buffer.push_back(tmp);
    tmp = data >> 16;
    this->_buffer.push_back(tmp);
    tmp = data >> 8;
    this->_buffer.push_back(tmp);
    tmp = data;
    this->_buffer.push_back(tmp);
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

void Stream::setDataLong(long data)
{
    unsigned char tmp = data >> 56;
    this->_buffer.push_back(tmp);
    tmp = data >> 48;
    this->_buffer.push_back(tmp);
    tmp = data >> 40;
    this->_buffer.push_back(tmp);
    tmp = data >> 32;
    this->_buffer.push_back(tmp);
    tmp = data >> 24;
    this->_buffer.push_back(tmp);
    tmp = data >> 16;
    this->_buffer.push_back(tmp);
    tmp = data >> 8;
    this->_buffer.push_back(tmp);
    tmp = data;
    this->_buffer.push_back(tmp);
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

Stream &Stream::operator<<(const Stream &stream)
{
    for (auto &c : stream.getBuffer())
        this->_buffer.push_back(c);
    return *this;
}

Stream &Stream::operator<<(u_char data)
{
    this->setDataUChar(data);
    return *this;
}

Stream &Stream::operator<<(u_short data)
{
    this->setDataUShort(data);
    return *this;
}

Stream &Stream::operator<<(u_int data)
{
    this->setDataUInt(data);
    return *this;
}

Stream &Stream::operator<<(u_long data)
{
    this->setDataULong(data);
    return *this;
}

Stream &Stream::operator<<(char data)
{
    this->setDataChar(data);
    return *this;
}

Stream &Stream::operator<<(short data)
{
    this->setDataShort(data);
    return *this;
}

Stream &Stream::operator<<(int data)
{
    this->setDataInt(data);
    return *this;
}

Stream &Stream::operator<<(long data)
{
    this->setDataLong(data);
    return *this;
}

Stream &Stream::operator<<(bool data)
{
    this->setDataUChar(data);
    return *this;
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
    out << 1_uc << progress;
    return out;
}

Stream StreamFactory::playerPos(u_int id, short x, short y)
{
    Stream out;
    out << 3_uc << id << x << y;
    return out;
}

Stream StreamFactory::missilePos(u_int id, u_char type, short x, short y)
{
    Stream out;
    out << 4_uc << id << type << x << y;
    return out;
}

Stream StreamFactory::score(int score)
{
    Stream out;
    out << 6_uc << score;
    return out;
}

Stream StreamFactory::monsterPos(u_int id, u_char type, short x, short y)
{
    Stream out;
    out << 7_uc << id << type << x << y;
    return out;
}

Stream StreamFactory::joinRoom(u_int roomId, u_int playerId)
{
    Stream out;
    out << 10_uc << roomId << playerId;
    return out;
}

Stream StreamFactory::waitGame(int time, bool start, unsigned char song)
{
    Stream out;
    out << 11_uc << time << start << song;
    return out;
}

Stream StreamFactory::playerJoinedGame(u_int playerId)
{
    Stream out;
    out << 13_uc << playerId;
    return out;
}

Stream StreamFactory::playerLeftGame(u_int playerId)
{
    Stream out;
    out << 14_uc << playerId;
    return out;
}

Stream StreamFactory::missileDestroyed(u_int id, u_char type, short x, short y)
{
    Stream out;
    out << 15_uc << id << type << x << y;
    return out;
}

Stream StreamFactory::monsterDied(u_int id)
{
    Stream out;
    out << 16_uc << id;
    return out;
}

Stream StreamFactory::gameOver(u_char type)
{
    Stream out;
    out << 17_uc << type;
    return out;
}

Stream StreamFactory::playerDied(u_int id)
{
    Stream out;
    out << 18_uc << id;
    return out;
}

Stream StreamFactory::playerLife(int life)
{
    Stream out;
    out << 19_uc << life;
    return out;
}

Stream StreamFactory::monsterLife(u_int id, int life)
{
    Stream out;
    out << 20_uc << id << life;
    return out;
}

Stream StreamFactory::askResend(u_short nbr)
{
    Stream out;
    out << 255_uc << nbr;
    return out;
}

Stream StreamFactory::strobe(u_char color, bool onOff)
{
    Stream out;
    out.setDataUChar(21);
    out.setDataUChar(color);
    out.setDataUChar(static_cast<u_char>(onOff));
    return out;
}

Stream StreamFactory::changeLevel(u_int timout, u_char song, bool started)
{
    Stream out;
    out.setDataUChar(22);
    out.setDataUInt(timout);
    out.setDataUChar(song);
    out.setDataUChar(static_cast<u_char>(started));
    return out;
}

Stream StreamFactory::bonusPos(u_int id, u_char type, short x, short y)
{
    Stream out;
    out.setDataUChar(28);
    out.setDataUInt(id);
    out.setDataUChar(type);
    out.setDataUShort(x);
    out.setDataUShort(y);
    return out;
}

Stream StreamFactory::bonusDestroyed(u_int id)
{
    Stream out;
    out.setDataUChar(29);
    out.setDataUInt(id);
    return out;
}

Stream StreamFactory::bombPos(u_int id, u_short x, u_short y)
{
    Stream out;
    out.setDataUChar(31);
    out.setDataUInt(id);
    out.setDataUShort(x);
    out.setDataUShort(y);
    return out;
}

Stream StreamFactory::bombDestroyed(u_int id)
{
    Stream out;
    out.setDataUChar(32);
    out.setDataUInt(id);
    return out;
}

Stream StreamFactory::podInfo(u_int userId, u_char lvl, u_char front)
{
    Stream out;
    out.setDataUChar(35);
    out.setDataUInt(userId);
    out.setDataUChar(lvl);
    out.setDataUChar(front);
    return out;
}

Stream StreamFactory::laserCreated(u_int id, u_short y)
{
    Stream out;
    out.setDataUChar(37);
    out.setDataUInt(id);
    out.setDataUShort(y);
    return out;
}

Stream StreamFactory::rayPos(u_int id, u_short x, u_short y)
{
    Stream out;
    out.setDataUChar(39);
    out.setDataUInt(id);
    out.setDataUShort(x);
    out.setDataUShort(y);
    return out;
}

Stream StreamFactory::BonusGet(u_char type)
{
    Stream out;
    out.setDataUChar(40);
    out.setDataUChar(type);
    return out;
}
#ifndef STREAM_HPP
#define STREAM_HPP

#include <vector>
#include <iostream>
#include <string>


class Stream
{
private:
    std::vector<unsigned char> _buffer;

public:
    Stream();
    Stream(const Stream &stream);
    Stream(const std::vector<unsigned char> &buffer);
    Stream(const std::string &str);
    Stream(const char str[], size_t size);
    ~Stream();

    const std::vector<unsigned char> &getBuffer() const;

    void operator += (const Stream &stream);
    void operator += (const unsigned char &data);
    void operator += (const std::string &str);

    Stream operator = (const Stream &stream);

    unsigned char operator[](size_t index) const;
    unsigned char &operator[](size_t index);

    void setDataCharArray(const char *data, size_t size);

    void setDataUInt(unsigned int data);
    void setDataUShort(unsigned short data);
    void setDataUChar(unsigned char data);
    void setDataInt(int data);
    void setDataShort(short data);
    void setDataChar(char data);

    size_t size() const;
    void clear();

    unsigned int getDataUInt();
    unsigned short getDataUShort();
    unsigned char getDataUChar();
    int getDataInt();
    short getDataShort();
    char getDataChar();

    Stream subStream(size_t pos) const;
    Stream subStream(size_t start, size_t size) const;
    std::string toString() const;

    class SubStreamError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };
};

std::ostream &operator<<(std::ostream &os,const Stream &steam);

namespace StreamFactory {
    Stream screenProgress(u_int progress);
    Stream playerPos(u_int id, short x, short y);
    Stream missilePos(u_int id, u_char type, short x, short y);
    Stream score(int score);
    Stream monsterPos(u_int id, u_char type, short x, short y);
    Stream joinRoom(u_int roomId, u_int playerId);
    Stream waitGame(int time, bool start);
    Stream playerJoinedGame(u_int playerId);
    Stream playerLeftGame(u_int playerId);
    Stream missileDestroyed(u_int id, u_char type, short x, short y);
    Stream monsterDied(u_int id);
    Stream gameOver(u_char type);
    Stream playerDied(u_int id);
    Stream playerLife(int life);
    Stream monsterLife(u_int id, int life);
}

#endif

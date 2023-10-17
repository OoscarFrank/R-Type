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

    static Stream toScreenProgress(u_int progress);
    static Stream toPlayerPos(u_int id, short x, short y);
    static Stream toMissilePos(u_int id, u_char type, short x, short y);
    static Stream toScore(int score);
    static Stream toMonsterPos(u_int id, short x, short y);
    static Stream toJoinRoom(u_int roomId, u_int playerId);
    static Stream toWaitGame(int time, bool start);
    static Stream toPlayerJoinedGame(u_int playerId);
    static Stream toPlayerLeftGame(u_int playerId);
    static Stream toMissileDestroyed(u_int id, u_char type, short x, short y);
    static Stream toMonsterDied(u_int id);
    static Stream toGameOver(u_char type);
    static Stream toPlayerDied(u_int id);
};

std::ostream &operator<<(std::ostream &os,const Stream &steam);

#endif

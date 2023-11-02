#pragma once

#include <vector>
#include <iostream>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#endif

namespace TypesLitterals {
    constexpr char operator""_c(unsigned long long c) { return static_cast<char>(c); }
    constexpr short operator""_s(unsigned long long c) { return static_cast<short>(c); }
    constexpr int operator""_i(unsigned long long c) { return static_cast<int>(c); }
    constexpr long operator""_l(unsigned long long c) { return static_cast<long>(c); }
    constexpr u_char operator""_uc(unsigned long long c) { return static_cast<u_char>(c); }
    constexpr u_short operator""_us(unsigned long long c) { return static_cast<u_short>(c); }
    constexpr u_int operator""_ui(unsigned long long c) { return static_cast<u_int>(c); }
    constexpr u_long operator""_ul(unsigned long long c) { return static_cast<u_long>(c); }
}

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

    void setDataULong(unsigned long data);
    void setDataUInt(unsigned int data);
    void setDataUShort(unsigned short data);
    void setDataUChar(unsigned char data);
    void setDataLong(long data);
    void setDataInt(int data);
    void setDataShort(short data);
    void setDataChar(char data);

    Stream &operator<<(const Stream &stream);
    Stream &operator<<(u_char data);
    Stream &operator<<(u_short data);
    Stream &operator<<(u_int data);
    Stream &operator<<(u_long data);
    Stream &operator<<(char data);
    Stream &operator<<(short data);
    Stream &operator<<(int data);
    Stream &operator<<(long data);
    Stream &operator<<(bool data);

    size_t size() const;
    void clear();

    unsigned long getDataULong();
    unsigned int getDataUInt();
    unsigned short getDataUShort();
    unsigned char getDataUChar();
    long getDataLong();
    int getDataInt();
    short getDataShort();
    char getDataChar();

    Stream &operator>>(u_char &data);
    Stream &operator>>(u_short &data);
    Stream &operator>>(u_int &data);
    Stream &operator>>(u_long &data);
    Stream &operator>>(char &data);
    Stream &operator>>(short &data);
    Stream &operator>>(int &data);
    Stream &operator>>(long &data);
    Stream &operator>>(bool &data);

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

std::ostream &operator<<(std::ostream &os,const Stream &stream);

namespace StreamFactory {
    Stream screenProgress(u_int progress);
    Stream playerPos(u_int id, short x, short y);
    Stream missilePos(u_int id, u_char type, short x, short y);
    Stream score(int score);
    Stream monsterPos(u_int id, u_char type, short x, short y);
    Stream joinRoom(u_int roomId, u_int playerId);
    Stream waitGame(int time, bool start, unsigned char song = 0);
    Stream playerJoinedGame(u_int playerId);
    Stream playerLeftGame(u_int playerId);
    Stream missileDestroyed(u_int id, u_char type, short x, short y);
    Stream monsterDied(u_int id);
    Stream gameOver(u_char type);
    Stream playerDied(u_int id);
    Stream playerLife(int life);
    Stream monsterLife(u_int id, int life);
    Stream askResend(u_short nbr);
    Stream strobe(u_char color, bool onOff);
    Stream changeLevel(u_int timout, u_char song, bool started);
    Stream bonusPos(u_int id,u_char type, short x, short y);
    Stream bonusDestroyed(u_int id);
    Stream bombPos(u_int id, u_short x, u_short y);
    Stream bombDestroyed(u_int id);
}

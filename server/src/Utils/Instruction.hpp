#pragma once

#include <vector>

#define CHAR    1
#define SHORT   2
#define INT     4
#define LONG    8

typedef struct {
    unsigned char _inst;
    std::vector<int> _datas;
    unsigned int _size;
}inst_t;

#define INST { \
    {0x01, {INT}, 4},\
    {0x02, {CHAR, CHAR}, 2}\
    {0x03, {CHAR, SHORT, SHORT}, 5}\
    {0x04, {CHAR, SHORT, SHORT}, 5}\
    {0x05, {}, 0}\
    {0x06, {INT}, 4}\
    {0x07, {CHAR, SHORT, SHORT}, 5}\
};
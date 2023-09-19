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
    {0x01, {INT, INT}, 8},\
};

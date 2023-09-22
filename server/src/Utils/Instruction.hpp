#pragma once

#include <vector>

#define CHAR    1
#define SHORT   2
#define INT     4

/**
 * @brief Commands
 * 0x01 serv - position x de l'ecran
 * 0x02 gui  - mouvement du player (1/2/3/4, nombre de fois)
 * 0x03 serv - position x et y du player (id, x, y)
 * 0x04 serv - position x et y d'un missile (id, x, y)
 * 0x05 gui  - spawn d'un missile par le player
 * 0x06 serv - score du joueur
 * 0x07 serv - position x et y d'un ennemi (id, x, y)
 * 0x08 gui  - creation d'une room (booleen pour savoir si la room est privee)
 * 0x09 gui  - match making
 * 0x0a serv - join d'une room (room id, player id)
 * 0x0b serv - temps restant avant le debut de la partie (temps, booleen pour savoir si la partie a commence)
 *
 */
struct Commands {
    unsigned char _inst;
    std::vector<int> _datas;
    unsigned int _size;
};

#define IN_COMMANDS { \
    {0x02, {CHAR, CHAR}, 2}, \
    {0x05, {}, 0}, \
    {0x08, {CHAR}, 1}, \
    {0x09, {}, 0}, \
};

#define OUT_COMMANDS { \
    {0x01, {INT}, 4}, \
    {0x03, {CHAR, SHORT, SHORT}, 5}, \
    {0x04, {CHAR, SHORT, SHORT}, 5}, \
    {0x06, {INT}, 4}, \
    {0x07, {CHAR, SHORT, SHORT}, 5}, \
    {0x0a, {SHORT, CHAR}, 3}, \
    {0x0b, {INT, CHAR}, 5} \
};

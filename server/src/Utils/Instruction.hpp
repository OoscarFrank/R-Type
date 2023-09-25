#pragma once

#include <vector>

#define CHAR    1
#define SHORT   2
#define INT     4

struct Commands {
    unsigned char _inst;
    std::vector<int> _datas;
    unsigned int _size;
};

#define SCREEN_HEIGHT       2160
#define SCREEN_WIDTH        3840

#define PLAYER_MOVE_OFFSET  1
#define PLAYER_MOVE_UP      1
#define PLAYER_MOVE_DOWN    2
#define PLAYER_MOVE_LEFT    3
#define PLAYER_MOVE_RIGHT   4

/**
 * @brief IN_COMMANDS
 * 0x02 gui - mouvement du player (1/2/3/4, nombre de fois) => 1 = haut, 2 = bas, 3 = gauche, 4 = droite
 * 0x05 gui - spawn d'un missile par le player
 * 0x08 gui - creation d'une room (booleen pour savoir si la room est privee)
 * 0x09 gui - match making
 *
 */
#define IN_COMMANDS { \
    {0x02, {CHAR, CHAR}, 2}, \
    {0x05, {}, 0}, \
    {0x08, {CHAR}, 1}, \
    {0x09, {}, 0}, \
};

/**
 * @brief Commands
 * 0x01 serv - progression de l'ecran
 * 0x03 serv - position x et y du player (id, x, y)
 * 0x04 serv - position x et y d'un missile (id, x, y)
 * 0x06 serv - score du joueur
 * 0x07 serv - position x et y d'un ennemi (id, x, y)
 * 0x0a serv - join d'une room (room id, player id)
 * 0x0b serv - temps restant avant le debut de la partie (temps, booleen pour savoir si la partie a commence)
 *
 */
#define OUT_COMMANDS { \
    {0x01, {INT}, 4}, \
    {0x03, {CHAR, SHORT, SHORT}, 5}, \
    {0x04, {CHAR, SHORT, SHORT}, 5}, \
    {0x06, {INT}, 4}, \
    {0x07, {CHAR, SHORT, SHORT}, 5}, \
    {0x0a, {SHORT, CHAR}, 3}, \
    {0x0b, {INT, CHAR}, 5} \
};

#pragma once

#include <vector>

#define CHAR    1
#define UCHAR   CHAR
#define SHORT   2
#define USHORT  SHORT
#define INT     4
#define UINT    INT

struct Commands {
    unsigned char _inst;
    std::vector<int> _datas;
    unsigned int _size;
    bool _important;
};

#define PLAYER_MOVE_UP      1
#define PLAYER_MOVE_DOWN    2
#define PLAYER_MOVE_LEFT    4
#define PLAYER_MOVE_RIGHT   8

#define SCREEN_WIDTH        3840
#define SCREEN_HEIGHT       2160

#define TIME_PLAYER_ALIVE   10 // seconds

/**
 * @brief IN_COMMANDS
 * 2 gui - mouvement du player (move, nombre de fois)
 * 5 gui - spawn d'un missile par le player
 * 8 gui - creation d'une room (booleen pour savoir si la room est privee)
 * 9 gui - match making
 * 12 gui - ping
 *
 */
#define OUT_COMMANDS { \
    {2, {UCHAR, UCHAR}, 2, false}, \
    {5, {}, 0, false}, \
    {8, {UCHAR}, 1, true}, \
    {9, {}, 0, true}, \
    {12, {}, 0, true}, \
    {255, {USHORT}, 2, true} \
}

/**
 * @brief Commands
 * 1 serv - progression de l'ecran
 * 3 serv - position x et y du player (id, x, y)
 * 4 serv - position x et y d'un missile (id,type, x, y)
 * 6 serv - score du joueur
 * 7 serv - position x et y d'un ennemi (id, type, x, y)
 * 10 serv - join d'une room (room id, player id)
 * 11 serv - temps restant avant le debut de la partie (temps, booleen pour savoir si la partie a commence)
 * 13 serv - player joined game (player id)
 * 14 serv - player left game (player id)
 * 15 serv - missile destroyed (id, type, x, y)
 * 16 serv - ennemi died (id)
 * 17 serv - game over (type)
 * 18 serv - player died (id)
 * 19 serv - vie du joueur
 * 20 serv - vie de l'ennemie (id)
 * 21 serv - strobes (color, on/off)
 */
#define IN_COMMANDS { \
    {1, {UINT}, 4, false}, \
    {3, {UINT, SHORT, SHORT}, 8, false}, \
    {4, {UINT, UCHAR, SHORT, SHORT}, 9, false}, \
    {6, {INT}, 4, false}, \
    {7, {UINT, UCHAR, SHORT, SHORT}, 9, false}, \
    {10, {UINT, UINT}, 8, true}, \
    {11, {INT, UCHAR, UCHAR}, 6, true}, \
    {13, {UINT}, 4, true}, \
    {14, {UINT}, 4, true}, \
    {15, {UINT, UCHAR, SHORT, SHORT}, 9, true}, \
    {16, {UINT}, 4, true}, \
    {17, {UCHAR}, 1, true}, \
    {18, {UINT}, 4, true}, \
    {19, {INT}, 4, true}, \
    {20, {UINT, INT}, 8, true}, \
    {21, {UCHAR, UCHAR}, 2, true}, \
    {255, {USHORT}, 2, true} \
}

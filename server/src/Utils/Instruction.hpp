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

#define SCREEN_WIDTH        3840
#define SCREEN_HEIGHT       2160

#define PLAYER_HEIGHT       65 * 2
#define PLAYER_WIDTH        113 * 2

#define TIME_PLAYER_ALIVE   10 // seconds

#define PLAYER_MOVE_OFFSET  10
#define PLAYER_MOVE_UP      1
#define PLAYER_MOVE_DOWN    2
#define PLAYER_MOVE_LEFT    4
#define PLAYER_MOVE_RIGHT   8

/**
 * @brief IN_COMMANDS
 * 2 gui - mouvement du player (1/2/3/4, nombre de fois) => 1 = haut, 2 = bas, 3 = gauche, 4 = droite
 * 5 gui - spawn d'un missile par le player
 * 8 gui - creation d'une room (booleen pour savoir si la room est privee)
 * 9 gui - match making
 * 12 gui - ping
 *
 */
#define IN_COMMANDS { \
    {2, {CHAR, CHAR}, 2}, \
    {5, {}, 0}, \
    {8, {CHAR}, 1}, \
    {9, {}, 0}, \
    {12, {}, 0} \
};

/**
 * @brief Commands
 * 1 serv - progression de l'ecran
 * 3 serv - position x et y du player (id, x, y)
 * 4 serv - position x et y d'un missile (id,type, x, y)
 * 6 serv - score du joueur
 * 7 serv - position x et y d'un ennemi (id, x, y)
 * 10 serv - join d'une room (room id, player id)
 * 11 serv - temps restant avant le debut de la partie (temps, booleen pour savoir si la partie a commence)
 * 13 serv - player joined game (player id)
 * 14 serv - player left game (player id)
 * 15 serv - missile destroyed (id, x, y)
 */
#define OUT_COMMANDS { \
    {1, {INT}, 4}, \
    {3, {CHAR, SHORT, SHORT}, 5}, \
    {4, {INT, CHAR, SHORT, SHORT}, 9}, \
    {6, {INT}, 4}, \
    {7, {CHAR, SHORT, SHORT}, 5}, \
    {10, {SHORT, CHAR}, 3}, \
    {11, {INT, CHAR}, 5}, \
    {13, {CHAR}, 1}, \
    {14, {CHAR}, 1}, \
    {15, {INT,CHAR, SHORT, SHORT}, 9} \
};




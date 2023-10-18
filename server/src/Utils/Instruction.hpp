#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

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
};

#define PLAYER_MOVE_UP      1
#define PLAYER_MOVE_DOWN    2
#define PLAYER_MOVE_LEFT    4
#define PLAYER_MOVE_RIGHT   8

/**
 * @brief IN_COMMANDS
 * 2 gui - mouvement du player (move, nombre de fois)
 * 5 gui - spawn d'un missile par le player
 * 8 gui - creation d'une room (booleen pour savoir si la room est privee)
 * 9 gui - match making
 * 12 gui - ping
 *
 */
#define IN_COMMANDS { \
    {2, {UCHAR, UCHAR}, 2}, \
    {5, {}, 0}, \
    {8, {UCHAR}, 1}, \
    {9, {}, 0}, \
    {12, {}, 0} \
};

/**
 * @brief Commands
 * 1 serv - progression de l'ecran
 * 3 serv - position x et y du player (id, type, x, y)
 * 4 serv - position x et y d'un missile (id,type, x, y)
 * 6 serv - score du joueur
 * 7 serv - position x et y d'un ennemi (id, x, y)
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
 */
#define OUT_COMMANDS { \
    {1, {UINT}, 4}, \
    {3, {UINT, UCHAR, SHORT, SHORT}, 8}, \
    {4, {UINT, SHORT, SHORT}, 9}, \
    {6, {INT}, 4}, \
    {7, {UINT, UCHAR, SHORT, SHORT}, 9}, \
    {10, {UINT, UINT}, 8}, \
    {11, {INT, UCHAR}, 5}, \
    {13, {UINT}, 4}, \
    {14, {UINT}, 4}, \
    {15, {UINT, UCHAR, SHORT, SHORT}, 9}, \
    {16, {UINT}, 4}, \
    {17, {UCHAR}, 1}, \
    {18, {UINT}, 4} \
    {19, {INT}, 4} \
    {20, {UINT, INT}, 8} \
};

#endif

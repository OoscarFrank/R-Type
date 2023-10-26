#ifndef SCHEDULING_HPP
#define SCHEDULING_HPP

#define SCREEN_WIDTH        3840
#define SCREEN_HEIGHT       2160

#define PLAYER_HEIGHT       65 * 2
#define PLAYER_WIDTH        113 * 2

#define LITTLE_MONSTER_HEIGHT      90
#define LITTLE_MONSTER_WIDTH       178

#define ZIGZAGER_MONSTER_HEIGHT    82
#define ZIGZAGER_MONSTER_WIDTH     183

#define FOLLOWER_MONSTER_HEIGHT    120
#define FOLLOWER_MONSTER_WIDTH     198

#define BURST_MONSTER_HEIGHT       120
#define BURST_MONSTER_WIDTH        198

#define BOSS1_MONSTER_HEIGHT       58 * 2
#define BOSS1_MONSTER_WIDTH        114 * 2

#define MISSILE_HEIGHT      1
#define MISSILE_WIDTH       1

// Progress steps
#define MAP_PROGRESS_STEP                       7

#define PLAYER_PROGRESS_STEP                    20
#define PLAYER_MISSILE_PROGRESS_STEP            50

#define LITTLE_MONSTER_PROGRESS_STEP            10
#define LITTLE_MONSTER_MISSILE_PROGRESS_STEP    30

#define ZIGZAGER_MONSTER_PROGRESS_STEP          10
#define ZIGZAGER_MONSTER_MISSILE_PROGRESS_STEP  30

#define FOLLOWER_MONSTER_PROGRESS_STEP          10
#define FOLLOWER_MONSTER_MISSILE_PROGRESS_STEP  30

// Times
#define MAP_REFRESH_TIME                10 // ms

#define PLAYER_MOVE_TIME                10 // ms
#define PLAYER_FIRE_TIME                150 // ms

#define MONSTER_MOVE_TIME               12 // ms
#define LITTLE_MONSTER_FIRE_TIME        1000 // ms
#define ZIGZAGER_MONSTER_FIRE_TIME      1000 // ms
#define FOLLOWER_MONSTER_FIRE_TIME      1000 // ms
#define BURST_MONSTER_FIRE_TIME         2000 // ms
#define BURST_FIRE_TIME                 300 // ms

#define MISSILES_MOVE_TIME              10 // ms

#define SEND_WAIT_MESSAGE_TIME          200 // ms
#define PLAYER_ALIVE_MAX_TIME           10 // seconds
#define TIMEOUT_START_GAME              1000 // ms
#define TIMEOUT_BETWEEN_LEVELS          5000 // ms
#define GAME_OVER_REFRESH               500 // ms

#endif

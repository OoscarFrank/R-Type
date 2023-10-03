#pragma once

#define SCREEN_WIDTH        3840
#define SCREEN_HEIGHT       2160

#define PLAYER_HEIGHT       65 * 2
#define PLAYER_WIDTH        113 * 2

#define LITTLE_MONSTER_HEIGHT      58 * 2
#define LITTLE_MONSTER_WIDTH       114 * 2

#define MISSILE_HEIGHT      1
#define MISSILE_WIDTH       1

#define MAP_PROGRESS_STEP               7
#define PLAYER_PROGRESS_STEP            10
#define PLAYER_MISSILE_PROGRESS_STEP    15
#define ENEMY_PROGRESS_STEP             1
#define ENEMY_MISSILE_PROGRESS_STEP     7

#define MAP_REFRESH_TIME                10 // ms
#define PLAYER_MOVE_TIME                5 // ms
#define PLAYER_FIRE_TIME                150 // ms
#define ENEMY_MOVE_TIME                 2 // ms
#define ENEMY_FIRE_TIME                 1000 // ms
#define ENEMY_SPAWN_TIME                1000 // ms
#define MISSILES_REFRESH_TIME           10 // ms
#define SEND_WAIT_MESSAGE_TIME          200 // ms
#define PLAYER_ALIVE_MAX_TIME           10 // seconds
#define TIMEOUT_START_GAME              3000 // ms
#define GAME_OVER_REFRESH               500 // ms

#pragma once
#include <math\math_types.h>
#include <utils\Color.h>

const int BULLET_TYPE       = 1;
const int ENEMY_TYPE        = 2;
const int PLAYER_TYPE       = 3;
const int POWERUP_TYPE      = 4;
const int NON_DEST_TYPE     = 5;
const int ENEMY_BULLET_TYPE = 6;
const int NO_POWERUP_TYPE   = 7;
const int SNAKE_HEAD        = 8;
const int SNAKE_TAIL        = 9;
const int SNAKE_SHIELD      = 10;
const int POWERUP_BASE      = 100;

const int GRID_SIZE = 32;
const int GRID_MAX_X = 32;
const int GRID_MAX_Y = 24;

const int MAX_SNAKE_TAILS = 10;

const int MAX_DODGERS = 32;

const int BG_LAYER      = 0;
const int LIGHT_LAYER   = 1;
const int MESSAGE_LAYER = 2;
const int OBJECT_LAYER  = 3;

const int MAX_BALLS = 256;

const int HUD_POINTS = 0;
const int HUD_LEVEL = 1;
const int HUD_TIMER = 2;

const float PLAYER_RADIUS = 22.0f;
const float BOMB_EXPLOSION_RADIUS = 100.0f;

const int SEEK_BIT = 1;
const int SEPARATE_BIT = 2;
const int ALIGN_BIT = 3;
const int SIMPLE_MOVE_BIT = 4;

const int BULLET_EXPLOSION = 1;
const int PLAYER_EXPLOSION = 5;
const int ENEMY_EXPLOSION = 0;
const int BALL_EXPLOSION = 7;
const int BOMB_STARTUP = 5;
const int BIG_CUBE_EXPLOSION = 6;
const int HUGE_CUBE_EXPLOSION = 6;

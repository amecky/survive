#pragma once
#include <math\math_types.h>
#include <utils\Color.h>

const int GRID_SIZE = 32;
const int GRID_MAX_X = 32;
const int GRID_MAX_Y = 24;

const int MAX_SNAKE_TAILS = 10;

const int MAX_DODGERS = 32;

const int BG_LAYER      = 0;
const int BORDER_LAYER  = 1;
const int MESSAGE_LAYER = 2;
const int OBJECT_LAYER  = 3;
const int EFFECT_LAYER  = 4;

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

const int ENEMY_TRAIL         = 3;
const int BULLET_EXPLOSION    = 2;
const int PLAYER_EXPLOSION    = 1;
const int ENEMY_EXPLOSION     = 1;
const int BALL_EXPLOSION      = 1;
const int BOMB_STARTUP        = 5;
const int BIG_CUBE_EXPLOSION  = 1;
const int HUGE_CUBE_EXPLOSION = 1;
const int EMITTER_EXPLOSION   = 4;
const int DEATHBALL_EXPLOSION = 6;
// objects types
enum ObjectTypes {
	OT_UNKNOWN,
	OT_BULLET,
	OT_PLAYER,
	OT_BOMB,
	OT_FOLLOWER,
	OT_BIG_CUBE,
	OT_HUGE_CUBE,
	OT_MEGA_CUBE,
	OT_SUPER_CUBE,
	OT_STAR,
	OT_CUBE_EMITTER,
	OT_RING,
	OT_DEATHBALL,
	OT_EOL
};

enum GridEdge {
	GE_TOP,
	GE_RIGHT,
	GE_BOTTOM,
	GE_LEFT,
	GE_EOL
};
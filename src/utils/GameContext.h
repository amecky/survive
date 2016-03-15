#pragma once
#include <world\World.h>
#include <particles\ParticleManager.h>
#include "SettingsConverter.h"
#include "..\Trail.h"
#include <dialogs\DialogManager.h>
#include <renderer\BitmapFont.h>


class GameRenderer;

struct GameContext {

	ds::World* world;
	GameSettings* settings;
	v2 playerPos;
	ds::SID playerID;
	ds::ParticleManager* particles;
	Trail* trails;
	ds::GUIDialog* hud;
	v2 world_pos;
	float playerSpeed;
	bool doubleFire;
	float fireRate;
	bool tripleShot;

	// score
	int kills;
	int points;
};
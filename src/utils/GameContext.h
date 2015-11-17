#pragma once
#include <world\World.h>
#include <particles\ParticleManager.h>
#include "SettingsConverter.h"
#include "..\Trail.h"
#include <dialogs\DialogManager.h>
#include <renderer\BitmapFont.h>
//#include "Lights.h"

//class FlashLights;

struct GameContext {

	ds::World* world;
	GameSettings* settings;
	v2 playerPos;
	ds::ParticleManager* particles;
	//ds::BitmapFont* font;
	Trail* trails;
	//ds::DialogManager* dialogs;
	//FlashLights* lights;
	ds::GUIDialog* hud;

	float playerSpeed;
	bool doubleFire;
	float fireRate;
	bool tripleShot;

	// score
	int kills;
	int points;
};
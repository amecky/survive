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
	Vector2f playerPos;
	ds::ParticleManager* particles;
	ds::BitmapFont* font;
	Trail* trails;
	ds::DialogManager* dialogs;
	//FlashLights* lights;

	float playerSpeed;
	bool doubleFire;
	float fireRate;
	bool tripleShot;

};
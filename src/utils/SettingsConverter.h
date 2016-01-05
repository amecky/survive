#pragma once
#include <data\DynamicSettings.h>

struct GameSettings : public ds::DynamicGameSettings {

	float bulletTrailDistance;
	float bulletVelocity;
	float spawnTimer;
	ds::Color snakeTailColor;
	float snakeVelocity;
	float snakeFollowVelocity;
	float shakePeriod;
	float shakeAmount;
	float shakeTTL;
	float playerFireRate;
	float warmUpTime;
	float getReadyAmplitude;
	float getReadyScale;
	float dyingTime;
	// border line settings
	float borderAmplitude;
	float borderShakeRadius;

	GameSettings() {
		addFloat("borderAmplitude", &borderAmplitude, 4.0f);
		addFloat("borderShakeRadius", &borderShakeRadius, 10.0f);
		addFloat("bulletTrailDistance", &bulletTrailDistance, 20.0f);
		addFloat("bulletVelocity", &bulletVelocity, 800.0f);
		addFloat("playerFireRate", &playerFireRate, 0.4f);
		addFloat("warmUpTime", &warmUpTime, 3.0f);
		addFloat("dyingTime", &dyingTime, 4.0f);
		addFloat("getReadyAmplitude", &getReadyAmplitude, 4.0f);
		addFloat("getReadyScale", &getReadyScale, 0.1f);
		//ds::Color snakeTailColor;
		addFloat("snakeVelocity", &snakeVelocity, 100.0f);
		addFloat("snakeFollowVelocity", &snakeFollowVelocity, 2.0f);
		addFloat("spawnTimer", &spawnTimer, 1.0f);
	}

	const char* getName() const {
		return "game_settings";
	}

};

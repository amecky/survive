#pragma once
#include <compiler\Converter.h>
#include <data\DataTranslator.h>
#include <io\Serializer.h>
#include <utils\FileUtils.h>
#include <ui\IMGUI.h>

struct GameSettings {

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
	float dodgersRelaxation;
	float dodgersSpawnDelay;
	float dodgersSpawnTimer;
	float dodgersVelocity;
	float warmUpTime;
	float getReadyAmplitude;
	float getReadyScale;
	float dyingTime;
	// border line settings
	float borderAmplitude;
	float borderShakeRadius;
	
	GameSettings() {
		borderAmplitude = 4.0f;
		borderShakeRadius = 10.0f;
		bulletTrailDistance = 20.0f;
		bulletVelocity = 800.0f;
	}

};

namespace settings {

	void save(GameSettings* settings);

	void load(GameSettings* settings);

	void showDialog(GameSettings* settings, int* states);
}

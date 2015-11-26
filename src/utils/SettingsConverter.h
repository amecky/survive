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
	
	/*
	"spawn_timer" : "1.5" ,
	"snake_tail_color" : "0,255,128,255" ,
	"snake_velocity" : "3.0" ,
	"snake_follow_velocity" : "8.0" ,
	"head_start_color" : "192,0,0,255" ,
	"head_kill_color" : "0,192,0,255" ,
	"shake_ttl" : "0.4" ,
	"shake_period" : "10" , 
	"shake_amount" : "6" ,
	"border_amplitude" : "4" ,
	"border_shake_radius" : "10"

	*/
	GameSettings() {
		addFloat("borderAmplitude", &borderAmplitude, 4.0f);
		addFloat("borderShakeRadius", &borderShakeRadius, 10.0f);
		addFloat("bulletTrailDistance", &bulletTrailDistance, 20.0f);
		addFloat("bulletVelocity", &bulletVelocity, 800.0f);
		addFloat("playerFireRate", &playerFireRate, 0.4f);
		addFloat("dodgersRelaxation", &dodgersRelaxation, 15.0f);
		addFloat("dodgersSpawnTimer", &dodgersSpawnTimer, 0.3f);
		addFloat("dodgersSpawnDelay", &dodgersSpawnDelay, 0.6f);
		addFloat("dodgersVelocity", &dodgersVelocity, 200.0f);
		addFloat("warmUpTime", &warmUpTime, 3.0f);
		addFloat("dyingTime", &dyingTime, 4.0f);
		addFloat("getReadyAmplitude", &getReadyAmplitude, 4.0f);
		addFloat("getReadyScale", &getReadyScale, 0.1f);
		//ds::Color snakeTailColor;
		addFloat("snakeVelocity", &snakeVelocity, 100.0f);
		addFloat("snakeFollowVelocity", &snakeFollowVelocity, 2.0f);
		addFloat("spawnTimer", &spawnTimer, 1.0f);
	}

};

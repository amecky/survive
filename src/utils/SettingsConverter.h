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

	int states[16];

	GameSettings() {
		for (int i = 0; i < 16; ++i) {
			states[i] = 0;
		}
		borderAmplitude = 4.0f;
		borderShakeRadius = 10.0f;
		bulletTrailDistance = 20.0f;
		bulletVelocity = 800.0f;
	}

	void save() {
		ds::file::saveBinary("Settings", this);
	}

	void load() {
		ds::file::loadBinary("Settings", this);
	}

	void showDialog() {
		gui::start(v2(100, 600));
		if (gui::begin("Bullet", &states[0])) {
			gui::InputFloat(3, "Trail distance", &bulletTrailDistance);
			gui::InputFloat(4, "Velocity", &bulletVelocity);
		}
		gui::end();

		if (gui::begin("Border", &states[1])) {
			gui::InputFloat(5, "Amplitude", &borderAmplitude);
			gui::InputFloat(6, "Shake radius", &borderShakeRadius);
			gui::beginGroup();
			if (gui::Button(1, "Save")) {
				LOG << "Save pressed";
			}
			if (gui::Button(2, "Load")) {
				LOG << "Load clicked";
			}
			gui::endGroup();
		}
		gui::end();
	}
};
/*
class SettingsLoader : public ds::Serializer, public ds::Converter {

public:
	SettingsLoader() : ds::Converter("SettingsLoader") {
		m_SettingsTranslator.add("bullet_trail_distance", &GameSettings::bulletTrailDistance);
		m_SettingsTranslator.add("bullet_velocity", &GameSettings::bulletVelocity);
		m_SettingsTranslator.add("spawn_timer", &GameSettings::spawnTimer);		
		m_SettingsTranslator.add("snake_tail_color", &GameSettings::snakeTailColor);
		m_SettingsTranslator.add("snake_velocity", &GameSettings::snakeVelocity);
		m_SettingsTranslator.add("shake_period", &GameSettings::shakePeriod);
		m_SettingsTranslator.add("shake_amount", &GameSettings::shakeAmount);
		m_SettingsTranslator.add("shake_ttl", &GameSettings::shakeTTL);
		m_SettingsTranslator.add("snake_follow_velocity", &GameSettings::snakeFollowVelocity);
		m_SettingsTranslator.add("player_fire_rate", &GameSettings::playerFireRate);
		m_SettingsTranslator.add("dodgers_relaxation", &GameSettings::dodgersRelaxation);
		m_SettingsTranslator.add("dodgers_spawn_delay", &GameSettings::dodgersSpawnDelay);
		m_SettingsTranslator.add("dodgers_spawn_timer", &GameSettings::dodgersSpawnTimer);
		m_SettingsTranslator.add("dodgers_velocity", &GameSettings::dodgersVelocity);
		m_SettingsTranslator.add("warm_up_time", &GameSettings::warmUpTime);
		m_SettingsTranslator.add("get_ready_amplitude", &GameSettings::getReadyAmplitude);
		m_SettingsTranslator.add("get_ready_scale", &GameSettings::getReadyScale);
		m_SettingsTranslator.add("dying_time", &GameSettings::dyingTime);
		m_SettingsTranslator.add("border_amplitude", &GameSettings::borderAmplitude);
		m_SettingsTranslator.add("border_shake_radius", &GameSettings::borderShakeRadius);
	}
	void load(BinaryLoader* loader);
	void convert(JSONReader& reader, BinaryWriter& writer);
	const char* getResourceDirectory() {
		return "content\\settings";
	}
	GameSettings* get() {
		return &m_Settings;
	}
private:
	GameSettings m_Settings;
	ds::DataTranslator<GameSettings> m_SettingsTranslator;
};
*/
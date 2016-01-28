#pragma once
#include <data\DynamicSettings.h>

struct CubeEmitterSettings : public ds::DynamicGameSettings {

	float scaleTTL;
	float spawnTTL;
	float ttl;
	ds::Color startFlash;
	ds::Color endFlash;
	float flashAmplitude;
	ds::Vector2fPath scalePath;

	CubeEmitterSettings() {
		addFloat("scale_ttl", &scaleTTL, 0.8f);
		addFloat("spawn_ttl", &spawnTTL, 0.8f);
		addFloat("ttl", &ttl, 1.2f);
		addFloat("scale_ttl", &scaleTTL, 0.4f);
		addColor("start_flash", &startFlash, ds::Color(181, 17, 68, 255));
		addColor("end_flash", &endFlash, ds::Color(181, 17, 68, 32));
		addFloat("flash_amplitude", &flashAmplitude, 0.4f);
		addPath("scale_path", &scalePath);
	}

	const char* getFileName() const {
		return "cube_emitter_settings.json";
	}

};

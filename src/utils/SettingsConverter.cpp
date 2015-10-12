#include "SettingsConverter.h"

namespace settings {

	void save(GameSettings* settings) {
		if (!ds::file::saveBinary<GameSettings>("Settings", settings)) {
			LOG << "Error saving file";
		}
	}

	void load(GameSettings* settings) {
		settings->borderAmplitude = 1.0f;
		if (!ds::file::loadBinary<GameSettings>("Settings", settings)) {
			LOG << "Error loading file";
		}
	}

	void showDialog(GameSettings* settings,v2* pos, int* states) {
		gui::start(pos);
		if (gui::begin("Bullet", &states[0])) {
			gui::InputFloat(3, "Trail distance", &settings->bulletTrailDistance);
			gui::InputFloat(4, "Velocity", &settings->bulletVelocity);
		}
		gui::end();

		if (gui::begin("Border", &states[1])) {
			gui::InputFloat(5, "Amplitude", &settings->borderAmplitude);
			gui::InputFloat(6, "Shake radius", &settings->borderShakeRadius);
			gui::beginGroup();
			if (gui::Button(1, "Save")) {
				LOG << "Save pressed";
				save(settings);
			}
			if (gui::Button(2, "Load")) {
				LOG << "Load clicked";
				load(settings);
			}
			gui::endGroup();
		}
		gui::end();
	}
}

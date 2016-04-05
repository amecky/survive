#include "Levels.h"

Levels::Levels() {
}


Levels::~Levels() {
}

// ---------------------------------------
// load levels definitions
// ---------------------------------------
bool Levels::loadData(const ds::JSONReader& reader) {
	int cats[64];
	int sub[64];
	int num = reader.get_categories(cats, 64);
	for (int i = 0; i < num; ++i) {
		int sub_num = reader.get_categories(sub, 64, cats[i]);
		for (int j = 0; j < sub_num; ++j) {
			LevelEntry def;
			def.level = i;
			reader.get_float(sub[j], "spawn_delay", &def.spawn_delay);
			reader.get_int(sub[j], "cube_type", &def.cube_type);
			reader.get_int(sub[j], "count", &def.count);
			_entries.push_back(def);
		}
	}
	return true;
}

// ---------------------------------------
// prepare level
// ---------------------------------------
void Levels::prepare(int level) {
	_timer = 0.0f;
	_current.clear();
	for (int i = 0; i < _entries.size(); ++i) {
		if (_entries[i].level == level) {
			_current.push_back(_entries[i]);
		}
	}
}


#pragma once
#include <io\DataFile.h>
#include <io\json.h>
#include "EventBuffer.h"

struct LevelEntry {
	float spawn_delay;
	int cube_type;
	int count;
	int level;
};

class Levels : public ds::DataFile {

public:
	Levels();
	~Levels();
	bool loadData(const ds::JSONReader& reader);
	const char* getFileName() const {
		return "resources\\waves.json";
	}
	void prepare(int level);
	void tick(EventBuffer& buffer,float dt);
private:
	ds::Array<LevelEntry> _current;
	ds::Array<LevelEntry> _entries;
	float _timer;
};


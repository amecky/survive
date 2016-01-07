#pragma once
#include <compiler\DataFile.h>
#include <lib\collection_types.h>

struct CubeDefinition {

	char name[20];
	float velocity;
	float velocityVariance;
	int numStars;
	int behaviorBits;
	float growTTL;
	int type;
	int energy;
	int nextType;
};

class CubeDefinitions : public ds::DataFile {

public:
	CubeDefinitions() {}
	~CubeDefinitions() {}
	bool saveData(JSONWriter& writer) {
		return true;
	}
	bool loadData(JSONReader& reader);
	const char* getFileName() const {
		return "resources\\cube_definitions.json";
	}
	size_t size() const {
		return _definitions.size();
	}
	const CubeDefinition& get(size_t index) const {
		return _definitions[index];
	}
private:
	ds::Array<CubeDefinition> _definitions;
};

struct WaveDefinition {
	int cubeType;
	int maxConcurrent;
	int numSpawn;
	float spawnTTL;	
};

class WaveDefinitions : public ds::DataFile {

public:
	WaveDefinitions() {}
	~WaveDefinitions() {}
	bool saveData(JSONWriter& writer) {
		return true;
	}
	bool loadData(JSONReader& reader);
	const char* getFileName() const {
		return "resources\\wave_definitions.json";
	}
	size_t size() const {
		return _definitions.size();
	}
	const WaveDefinition& get(size_t index) const {
		return _definitions[index];
	}
private:
	ds::Array<WaveDefinition> _definitions;
};

class Cubes
{
public:
	Cubes();
	~Cubes();
};

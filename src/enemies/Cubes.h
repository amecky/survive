#pragma once
#include <io\DataFile.h>
#include <io\json.h>
#include <lib\collection_types.h>
#include <sprites\SpriteBatch.h>
#include "..\utils\GameContext.h"
#include <lib\DataArray.h>
#include "BallEmitter.h"
#include "..\utils\CubeEmitterSettings.h"

// ---------------------------------------
// Cube definition
// ---------------------------------------
struct CubeDefinition {

	char name[20];
	float velocity;
	float velocityVariance;
	int numStars;
	float growTTL;
	int type;
	int energy;
	int nextType;
	float trailDistance;
	int trailSystem;
};

// ---------------------------------------
// Cube definitions
// ---------------------------------------
class CubeDefinitions : public ds::DataFile {

public:
	CubeDefinitions() {}
	~CubeDefinitions() {}
	bool saveData(ds::JSONWriter& writer) {
		return true;
	}
	bool loadData(const ds::JSONReader& reader);
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

// ---------------------------------------
// Wave definition
// ---------------------------------------
struct WaveDefinition {
	int cubeType;
	int maxConcurrent;
	int numSpawn;
	float spawnTTL;	
};

// ---------------------------------------
// Wave definitions
// ---------------------------------------
class WaveDefinitions : public ds::DataFile {

public:
	WaveDefinitions() {}
	~WaveDefinitions() {}
	bool saveData(ds::JSONWriter& writer) {
		return true;
	}
	bool loadData(const ds::JSONReader& reader);
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

struct KilledBall {

	v2 position;
	int type;

};

// ----------------------------------------
// Ball
// ----------------------------------------
struct Ball {

	v2 velocity;
	v2 force;
	int def_index;
	int wave_index;
	int energy;
	ds::SID lightIndex;

	Ball() : velocity(0, 0), force(0, 0) , def_index(0) , wave_index(0) , energy(0) , lightIndex(ds::INVALID_SID) {}

};

// ---------------------------------------
// Wave runtime
// ---------------------------------------
struct WaveRuntime {
	int definitionIndex;
	int current;
	int total;
	float timer;
};

typedef ds::Array<WaveRuntime> WaveRuntimes;

struct CubeEmitter {

	float timer;
	float ttl;
	float spawnTTL;
	int cube_type;
	bool dead;

};

// ---------------------------------------
// Cubes
// ---------------------------------------
class Cubes {

public:
	Cubes(GameContext* context);
	~Cubes();
	void activate();
	void spawn(float dt);
	int killBalls(const v2& bombPos, KilledBall* positions);
	void killAll(bool explode = true);
	void emitt(int type);
	void handleEvents(const ds::ActionEventBuffer& buffer);
	void move(float dt);
	int kill(ds::SID sid);
	void reload();
private:	
	void createBall(const v2& pos, int current, int total, int waveDefinitionIndex);
	void seek(const v2& target, float velocity);
	void separate(const v2& target, float minDistance, float relaxation);
	void align(const v2& target, float desiredDistance);
	GameContext* _context;
	ds::World* _world;
	v2 _spawner_position;
	SpawnerData _spawnData;
	CubeDefinitions _cubeDefintions;
	WaveDefinitions _waveDefinitions;
	WaveRuntimes _waveRuntimes;
	StartPoints _startPoints;
	BallEmitter* _emitter;
	int _killed;
	int _emitted;
	CubeEmitterSettings _cubeEmitterSettings;
};

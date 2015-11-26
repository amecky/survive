#pragma once
#include "Enemies.h"
#include <compiler\DataFile.h>
#include "EnemySpawner.h"

const int MAX_WAVES = 16;
// -------------------------------------------
// wave definition
// -------------------------------------------
struct WaveDefinition {

	int index;
	EnemyType enemy_type;
	SpawnerData spawner_data;
	int count;
	int health;
	float delay;
	int next;
	bool used;

	WaveDefinition() : index(-1) , enemy_type(ET_EOL), spawner_data() , count(0), health(0), delay(0.0f), next(-1) , used(false) {}

	WaveDefinition(int _index, EnemyType _type, const SpawnerData& data,int _count, int _health, float _delay, int _next) 
		: index(_index), enemy_type(_type), spawner_data(data) , count(_count), health(_health), delay(_delay), next(_next) , used(true) {}

};

// -------------------------------------------
// wave
// -------------------------------------------
struct Wave {
	Enemies* enemies;
	int count;
	bool immedate;
	float timer;
	int health;
	bool active;
	int killCounter;
};

// -------------------------------------------
// impact type
// -------------------------------------------
enum ImpactType {
	IMT_NONE,
	IMT_KILLED,
	IMT_NEXT_WAVE,
	IMT_NEXT_STAGE,
	IMT_EOL
};

// -------------------------------------------
// impact event
// -------------------------------------------
struct ImpactEvent {

	ImpactType type;
	int points;

};

// -------------------------------------------
// stage 
// -------------------------------------------
struct Stage {

	WaveDefinition waves[MAX_WAVES];
	int count;
	int index;
	float delays[16];

	Stage() : count(0) , index(0) {
		reset();
	}

	void reset() {
		count = 0;
		index = 0;
		for (int i = 0; i < 16; ++i) {
			delays[i] = 0.0f;
			waves[i].used = false;
		}
	}
};

// -------------------------------------------
// stage manager
// -------------------------------------------
class StageManager : public ds::DataFile {

public:
	StageManager(GameContext* context);
	~StageManager();
	void start();
	void addStage(int idx,const WaveDefinition& def);
	void setWave(int stageIndex, const WaveDefinition& def);
	bool tick(float dt);
	void killAll();
	int handleImpact(ds::SID id);

	void startWave(const WaveDefinition& def);
	void handleEvents(const ds::ActionEventBuffer& buffer);
	bool exportData(JSONWriter& writer);
	bool importData(JSONReader& reader);
	bool saveData(BinaryWriter& writer);
	bool loadData(BinaryLoader& loader);
	const char* getJSONFileName() const {
		return "stages.json";
	}
	const char* getFileName() const {
		return "stages";
	}
private:
	int findFreeSlot() const;
	bool hasParent(int index) const;
	void startWaves(int stageIndex);
	bool next();

	Stage _stages[16];
	Wave _activesWaves[16];
	int _stageCount;
	int _index;
	GameContext* _context;
	int _killCounter;
};


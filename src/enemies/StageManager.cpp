#include "StageManager.h"
#include "Dodgers.h"
#include "Bouncer.h"
#include "Snake.h"
#include <utils\Log.h>

// -------------------------------------------
// stage manager
// -------------------------------------------
StageManager::StageManager(GameContext* context) : _context(context) , _index(0) , _killCounter(0) {
	SpawnerData data;
	data.count_x = 10;
	data.count_y = 5;
	data.border = v2(40, 40);
	for (int i = 0; i < MAX_WAVES; ++i) {
		_activesWaves[i].active = false;
	}
}


StageManager::~StageManager() {
}

// -------------------------------------------
// set wave
// -------------------------------------------
void StageManager::setWave(int stageIndex, const WaveDefinition& def) {
	_stages[stageIndex].waves[def.index] = def;
}

// -------------------------------------------
// add stage
// -------------------------------------------
void StageManager::addStage(int idx, const WaveDefinition& def) {
	_stages[idx].waves[_stages[idx].count++] = def;
}

// -------------------------------------------
// start
// -------------------------------------------
void StageManager::start() {
	LOGC("StageManager") << "start";
	_index = 0;
	_killCounter = 0;
	_stages[0].index = 0;	
	startWaves(_index);
}

// -------------------------------------------
// has parent
// -------------------------------------------
bool StageManager::hasParent(int index) const {
	const Stage& stage = _stages[_index];
	for (int i = 0; i < stage.count; ++i) {
		const WaveDefinition& def = stage.waves[i];
		if ( def.index != index ) {
			if (def.next == index) {
				return true;
			}
		}
	}
	return false;
}

// -------------------------------------------
// find free slot
// -------------------------------------------
int StageManager::findFreeSlot() const {
	for (int i = 0; i < MAX_WAVES; ++i) {
		if (!_activesWaves[i].active) {
			return i;
		}
	}
	return -1;
}

// -------------------------------------------
// start waves
// -------------------------------------------
void StageManager::startWaves(int stageIndex) {
	LOGC("StageManager") << "start waves - stage: " << stageIndex;
	const Stage& stage = _stages[_index];
	for (int i = 0; i < stage.count; ++i) {
		const WaveDefinition& def = stage.waves[i];
		// def has no parent -> start
		if (!hasParent(def.index)) {
			LOGC("StageManager") << def.index << " has no parent";
			int idx = findFreeSlot();
			LOGC("StageManager") << "free slot: " << idx;
			if (idx != -1) {
				Wave& w = _activesWaves[idx];
				//w.enemies = new Dodgers(_context);
				w.count = def.count;
				w.enemies->activate(def.count);
				w.immedate = true;
				w.health = def.health;
				w.active = true;
				w.killCounter = 0;
			}
		}
	}
}

// -------------------------------------------
// start waves
// -------------------------------------------
void StageManager::startWave(const WaveDefinition& def) {
	int idx = findFreeSlot();
	LOGC("StageManager") << "free slot: " << idx;
	if (idx != -1) {
		Wave& w = _activesWaves[idx];
		if (def.enemy_type == ET_BOUNCER) {
			w.enemies = new Bouncer(_context, def.spawner_data);
		}
		else if (def.enemy_type == ET_DODGERS) {
			w.enemies = new Dodgers(_context, def.spawner_data);
		}
		else if (def.enemy_type == ET_SNAKE) {
			w.enemies = new Snake(_context, def.spawner_data);
		}
		w.count = def.count;
		w.enemies->activate(def.count);
		w.immedate = true;
		w.health = def.health;
		w.active = true;
		w.killCounter = 0;
	}
}

void StageManager::handleEvents(const ds::ActionEventBuffer& buffer) {
	for (int i = 0; i < MAX_WAVES; ++i) {
		if (_activesWaves[i].active) {
			_activesWaves[i].enemies->handleEvents(buffer);
		}
	}
}
// -------------------------------------------
// tick
// -------------------------------------------
bool StageManager::tick(float dt) {
	for (int i = 0; i < MAX_WAVES; ++i) {
		if (_activesWaves[i].active) {
			_activesWaves[i].enemies->tick(dt);
		}
	}
	return false;
}

// -------------------------------------------
// handle impact
// -------------------------------------------
int StageManager::handleImpact(ds::SID id) {
	for (int i = 0; i < MAX_WAVES; ++i) {
		Wave& w = _activesWaves[i];
		if (w.active) {
			Enemies* enemies = w.enemies;
			if (enemies->handleImpact(id)) {
				++w.killCounter;
				if (w.killCounter >= w.count) {
					LOG << "ALL KILLED";
					w.active = false;
					delete w.enemies;
					// FIXME: move to next or end stage
				}
				return 100;
			}
		}
	}
	return 0;
}

// -------------------------------------------
// kill all
// -------------------------------------------
void StageManager::killAll() {
	for (int i = 0; i < MAX_WAVES; ++i) {
		if (_activesWaves[i].active) {
			_activesWaves[i].enemies->killAll();
			delete _activesWaves[i].enemies;
			_activesWaves[i].active = false;
		}
	}
}

// -------------------------------------------
// next
// -------------------------------------------
bool StageManager::next() {
	++_index;
	if (_index >= 1) {
		_index = -1;
		return false;
	}
	return true;
}

// -------------------------------------------
// export data
// -------------------------------------------
bool StageManager::exportData(JSONWriter& writer) {
	return true;
}

/*
"stage" {
"index" : "0" ,
"wave" : { "index" : "0" , "enemy_index" : "0" , "count" : "20" , "health" : "1" , "delay" :   "0" , "next" :  "1" } ,
"wave" : { "index" : "1" , "enemy_index" : "0" , "count" : "20" , "health" : "2" , "delay" :   "0" , "next" : "-1" } ,
"wave" : { "index" : "2" , "enemy_index" : "1" , "count" :  "8" , "health" : "4" , "delay" : "5.0" , "next" : "-1" }
}

*/
// -------------------------------------------
// import data
// -------------------------------------------
bool StageManager::importData(JSONReader& reader) {
	// FIXME: reset
	std::vector<Category*> categories = reader.getCategories();
	for (size_t i = 0; i < categories.size(); ++i) {
		Category* c = categories[i];
		LOG << "name: " << c->getName();
		if (c->getName() == "stage") {
			int idx = c->getInt("index", -1);
			if (idx != -1) {
				std::vector<Category*> children = c->getChildren();
				for (size_t j = 0; j < children.size(); ++j) {
					Category* w = children[j];
					if (w->getName() == "wave") {
						WaveDefinition def;
						int widx = w->getInt("index", -1);
						if (widx != -1) {
							def.index = widx;
							//w->getInt("enemy_index", &def.enemy_index);
							w->getInt("count", &def.count);
							w->getInt("health", &def.health);
							w->getFloat("delay", &def.delay);
							w->getInt("next", &def.next);
							def.used = true;
							setWave(idx, def);
						}
					}
				}
			}
		}
		
	}
	return true;
}

// -------------------------------------------
// save data
// -------------------------------------------
bool StageManager::saveData(BinaryWriter& writer) {
	return true;
}

// -------------------------------------------
// load data
// -------------------------------------------
bool StageManager::loadData(BinaryLoader& loader) {
	return true;
}

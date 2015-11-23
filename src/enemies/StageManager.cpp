#include "StageManager.h"
#include "Dodgers.h"

StageManager::StageManager(GameContext* context) : _context(context) , _index(0) , _killCounter(0) {
	_enemyList.push_back(new Dodgers(_context));
}


StageManager::~StageManager() {
	for (size_t i = 0; i < _enemyList.size(); ++i) {
		delete _enemyList[i];
	}
}

void StageManager::setWave(int stageIndex, int waveIndex, const WaveDefinition& def) {
	_stages[stageIndex].waves[waveIndex] = def;
}

void StageManager::addStage(int idx, const WaveDefinition& def) {
	_stages[idx].waves[_stages[idx].count++] = def;
}

void StageManager::start() {
	_index = 0;
	_killCounter = 0;
	_stages[0].index = 0;	
	const WaveDefinition& def = getCurrentDefinition();
	Enemies* enemies = _enemyList[def.enemyIndex];
	enemies->activate(def.count);
}

const WaveDefinition&  StageManager::getCurrentDefinition() const {
	const Stage& stage = _stages[_index];
	return stage.waves[stage.index];
}

bool StageManager::tick(float dt) {
	if (_index != -1) {
		const Stage& stage = _stages[_index];
		const WaveDefinition& def = stage.waves[stage.index];
		Enemies* enemies = _enemyList[def.enemyIndex];
		enemies->tick(dt);
	}
	return false;
}

int StageManager::handleImpact(ds::SID id) {
	const WaveDefinition& def = getCurrentDefinition();
	Enemies* enemies = _enemyList[def.enemyIndex];
	if (enemies->handleImpact(id)) {
		++_killCounter;
		const Stage& stage = _stages[_index];
		const WaveDefinition& def = stage.waves[stage.index];
		if (_killCounter >= def.count) {
			LOG << "ALL KILLED";
			if (next()) {

			}
			// move to next or end stage
		}
		return 100;
	}
	return 0;
}

void StageManager::killAll() {

}

bool StageManager::next() {
	++_index;
	if (_index >= 1) {
		_index = -1;
		return false;
	}
	return true;
}

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
							w->getInt("enemy_index", &def.enemyIndex);
							w->getInt("count", &def.count);
							w->getInt("health", &def.health);
							w->getFloat("delay", &def.delay);
							w->getInt("next", &def.next);
							def.used = true;
							setWave(idx, widx, def);
						}
					}
				}
			}
		}
		
	}
	return true;
}

bool StageManager::saveData(BinaryWriter& writer) {
	return true;
}

bool StageManager::loadData(BinaryLoader& loader) {
	return true;
}

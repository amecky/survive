#include "Enemies.h"

Enemies::Enemies(GameContext* context, const SpawnerData& data) : _context(context) , _spawnTimer(0.0f) , _maxEnemies(0) , _counter(0) {
	createSpawner(data);
	_spawner->rebuild();
}

Enemies::~Enemies() {
	delete _spawner;
}
// ------------------------------------------------
// check if this is a enemy
// ------------------------------------------------
const bool Enemies::contains(ds::SID sid) const {
	for (int i = 0; i < _list.size(); ++i) {
		if (_list[i].sid == sid) {
			return true;
		}
	}
	return false;
}

// ------------------------------------------------
// remove enemy
// ------------------------------------------------
void Enemies::remove(ds::SID sid) {
	EnemyList::iterator it = _list.begin();
	while (it != _list.end()) {
		if (it->sid == sid) {
			_context->world->remove(sid);
			_context->trails->remove(sid);
			if (it->lightID != ds::INVALID_SID) {
				_context->world->remove(it->lightID);
			}
			it = _list.erase(it);
		}
		else {
			++it;
		}
	}
}

void Enemies::createSpawner(const SpawnerData& data) {
	assert(data.type != SPT_EOL);
	_spawnData = data;
	if (data.type == SPT_PARTIAL_EDGES) {
		_spawner = new EdgesSpawner(_spawnData);
	}
}

// ------------------------------------------------
// tick and create new dodgers
// ------------------------------------------------
void Enemies::tick(float dt) {
	if (_spawnData.emitter_type == SET_DELAYED) {
		_spawnTimer += dt;
		if (_spawnTimer > _spawnData.delay) {
			_spawnTimer = 0.0f;
			if (_counter < _maxEnemies){
				StartPoint sp;
				const SpawnPoint& spawn = _spawner->next();
				sp.position = spawn.position;
				sp.normal = spawn.normal;
				sp.timer = 0.0f;
				_startPoints.push_back(sp);
				// FIXME: call back for emitt particles
				//_context->particles.start(9, sp.position);
				_context->particles->start(10, sp.position);
				++_counter;
			}
		}
	}
	if (!_startPoints.empty()) {
		StartPoints::iterator it = _startPoints.begin();
		while (it != _startPoints.end()) {
			it->timer += dt;
			if (it->timer >= _context->settings->dodgersSpawnDelay) {
				create(*it);
				it = _startPoints.erase(it);
			}
			else {
				++it;
			}
		}
	}
	move(dt);
}

// ------------------------------------------------
// start timer
// ------------------------------------------------
void Enemies::activate(int maxEnemies) {
	_spawnTimer = 0.0f;
	_spawner->rebuild();
	_maxEnemies = maxEnemies;
	_counter = 0;
	if (_spawnData.emitter_type == SET_IMMEDIATE) {
		for (int i = 0; i < _maxEnemies; ++i) {
			StartPoint sp;
			const SpawnPoint& spawn = _spawner->next();
			sp.position = spawn.position;
			sp.normal = spawn.normal;
			sp.timer = 0.0f;
			_startPoints.push_back(sp);
			// FIXME: call back for emitt particles
			//_context->particles.start(9, sp.position);
			_context->particles->start(10, sp.position);
		}
	}
}

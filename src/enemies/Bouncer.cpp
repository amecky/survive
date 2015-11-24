#include "Bouncer.h"
#include "..\Constants.h"

Bouncer::Bouncer(GameContext* context) : ds::TimedObject() ,Enemies(context) {
}

Bouncer::~Bouncer() {

}
// ------------------------------------------------
// create dodger
// ------------------------------------------------
void Bouncer::create(const Vector2f& start) {
	if (_counter < _maxEnemies) {
		Enemy d;
		d.sid = _context->world->create(start, "dodger", OBJECT_LAYER);
		//_context->world->setColor(d.sid, ds::Color(255,0,0,255));
		_context->world->attachCollider(d.sid, SNAKE_TAIL, OBJECT_LAYER);
		d.lightID = _context->world->create(start, "lightning", LIGHT_LAYER);
		d.timer = 0.0f;
		d.position = start;
		d.angle = ds::math::getTargetAngle(_context->playerPos, start);
		_context->world->setColor(d.lightID, ds::Color(50, 213, 255, 255));
		_context->world->scale(d.lightID, 0.7f, 0.7f);
		_context->world->setRotation(d.sid, d.angle);
		_list.push_back(d);
		_context->trails->add(d.sid, 5.0f, 11);
		++_counter;
	}
}

// ------------------------------------------------
// kill all
// ------------------------------------------------
void Bouncer::killAll() {
	EnemyList::iterator it = _list.begin();
	while (it != _list.end()) {
		if (_context->world->contains(it->sid)) {
			Vector2f enemyPos = _context->world->getPosition(it->sid);
			_context->world->remove(it->sid);
			_context->trails->remove(it->sid);
			_context->particles->start(0, enemyPos);
			_context->particles->start(2, enemyPos);
			_context->world->remove(it->lightID);
		}
		it = _list.erase(it);
	}
}

// ------------------------------------------------
// start timer
// ------------------------------------------------
void Bouncer::activate(int maxEnemies) {
	startTimer();
	_spawner.minSpawns = 4;
	_spawner.maxSpawns = 8;
	_spawner.totalSpawns = ds::math::random(_spawner.minSpawns, _spawner.maxSpawns);
	_maxEnemies = maxEnemies;
	_counter = 0;
}

// ------------------------------------------------
// move
// ------------------------------------------------
void Bouncer::move(float dt) {
	v2 v;
	for (size_t i = 0; i < _list.size(); ++i) {
		Enemy& d = _list[i];
		d.position += d.velocity * dt;
		d.angle = ds::math::getTargetAngle(d.velocity, V2_RIGHT);
		_context->world->setRotation(d.sid, d.angle);
		_context->world->setPosition(d.sid, d.position);
		_context->world->setPosition(d.lightID, d.position);
	}
}

// ------------------------------------------------
// tick and create new dodgers
// ------------------------------------------------
void Bouncer::tick(float dt) {
	if (tickTimer(dt, _context->settings->dodgersSpawnTimer, true)) {
		if (_counter < _maxEnemies){
			StartPoint sp;
			sp.position = _spawner.pick();
			sp.timer = 0.0f;
			_startPoints.push_back(sp);
			//_context->particles.start(9, sp.position);
			_context->particles->start(10, sp.position);
		}
	}
	StartPoints::iterator it = _startPoints.begin();
	while (it != _startPoints.end()) {
		it->timer += dt;
		if (it->timer >= _context->settings->dodgersSpawnDelay) {
			create(it->position);
			it = _startPoints.erase(it);
		}
		else {
			++it;
		}
	}
	move(dt);
}

bool Bouncer::handleImpact(ds::SID sid) {
	if (contains(sid)) {
		v2 enemyPos = _context->world->getPosition(sid);
		_context->particles->start(0, enemyPos);
		_context->particles->start(2, enemyPos);
		remove(sid);
		return true;
	}
	return false;
}

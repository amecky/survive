#include "Dodgers.h"
#include "..\Constants.h"

Dodgers::Dodgers(GameContext* context) : ds::TimedObject() , Enemies(context) {
}

Dodgers::~Dodgers() {

}
// ------------------------------------------------
// create dodger
// ------------------------------------------------
void Dodgers::create(const Vector2f& start) {
	if (_counter < _maxEnemies) {
		Dodger d;
		d.sid = _context->world->create(start, "dodger", OBJECT_LAYER);
		//_context->world->setColor(d.sid, ds::Color(255,0,0,255));
		_context->world->attachCollider(d.sid, SNAKE_TAIL, OBJECT_LAYER);
		d.lightID = _context->world->create(start, "lightning", LIGHT_LAYER);
		d.timer = 0.0f;
		d.wobble = 0.0f;
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
// remove dodger
// ------------------------------------------------
void Dodgers::remove(ds::SID sid) {
	DodgerList::iterator it = _list.begin();
	while (it != _list.end()) {
		if (it->sid == sid) {
			_context->world->remove(sid);
			_context->trails->remove(sid);			
			_context->world->remove(it->lightID);
			it = _list.erase(it);
		}
		else {
			++it;
		}
	}
}

// ------------------------------------------------
// kill all
// ------------------------------------------------
void Dodgers::killAll() {
	DodgerList::iterator it = _list.begin();
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
// check if this is a dodger
// ------------------------------------------------
const bool Dodgers::contains(ds::SID sid) const {
	for (int i = 0; i < _list.size(); ++i) {
		if (_list[i].sid == sid) {
			return true;
		}
	}
	return false;
}

// ------------------------------------------------
// start timer
// ------------------------------------------------
void Dodgers::activate(int maxEnemies) {
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
void Dodgers::move(float dt) {
	/*
	float angle = 0.0f;
	for (size_t i = 0; i < _list.size(); ++i) {
		_list[i].timer += dt;
		_list[i].wobble = sin(_list[i].timer);
		if (_list[i].wobble < 0.0f) {
			_list[i].wobble *= -1.0f;
		}
	}
	*/
	v2 v;
	for (size_t i = 0; i < _list.size(); ++i) {
		Dodger& d = _list[i];
		v2 acceleration = v2(0, 0);
		acceleration += seek(i, _context->playerPos);
		acceleration += align(i);
		acceleration += separate(i);
		d.velocity += acceleration * dt;
		d.position += d.velocity * dt;
		d.angle = ds::math::getTargetAngle(d.velocity, V2_RIGHT);
		_context->world->setRotation(d.sid, d.angle);
		_context->world->setPosition(d.sid, d.position);
		_context->world->setPosition(d.lightID, d.position);
	}

	/*
	for (size_t i = 0; i < _list.size(); ++i) {
		ds::SID sid = _list[i].sid;
		Vector2f p = _context->world->getPosition(sid);
		angle = ds::math::reflect(ds::math::getTargetAngle(_context->playerPos, p));
		//angle += _list[i].wobble;
		_list[i].velocity = ds::math::getRadialVelocity(angle, _context->settings->dodgersVelocity);
		//ds::math::move_towards(_context->playerPos, p, &angle, _context->settings->dodgersVelocity, dt);
		p += _list[i].velocity * dt;

		_context->world->setRotation(sid, angle);// ds::math::reflect(angle));
		ds::SID closest = findNearest(p, 22.0f, sid);
		if (closest != ds::INVALID_SID) {
			Vector2f cp = _context->world->getPosition(closest);
			Vector2f diff = p - cp;
			Vector2f n = normalize(diff);
			n *= _context->settings->dodgersRelaxation;
			p += n * dt;
			cp -= n * dt;
			_context->world->setPosition(closest, cp);

		}
		_context->world->setPosition(_list[i].lightID, p);
		_context->world->setPosition(sid, p);
	}
	*/
}

// ------------------------------------------------
// tick and create new dodgers
// ------------------------------------------------
void Dodgers::tick(float dt) {
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

// -------------------------------------------------------
// find nearest enemy to current enemy
// -------------------------------------------------------
ds::SID Dodgers::findNearest(const Vector2f& pos, float radius,ds::SID self) {
	ds::SID ret = ds::INVALID_SID;
	float min = 2000.0f * 2000.0f;
	for (int i = 0; i < _list.size(); ++i) {
		if (_list[i].sid != self) {
			Vector2f diff = pos - _context->world->getPosition(_list[i].sid);
			float distSq = sqr_length(diff);
			if (distSq < (radius + radius)*(radius + radius)) {
				if (distSq < min) {
					ret = _list[i].sid;
					min = distSq;
				}
			}
		}
	}
	return ret;
}

bool Dodgers::handleImpact(ds::SID sid) {
	if (contains(sid)) {		
		v2 enemyPos = _context->world->getPosition(sid);
		_context->particles->start(0, enemyPos);
		_context->particles->start(2, enemyPos);
		remove(sid);
		return true;
	}
	return false;
}

v2 Dodgers::seek(int index, const v2& target) {
	v2 desired = target - _list[index].position;
	// Scale to maximum speed
	normalize(desired);
	desired.x *= 1.25f;
	desired.y *= 1.25f;
	// Steering = Desired minus Velocity
	v2 steer = desired - _list[index].velocity;
	return steer;
}

v2 Dodgers::align(int index) {
	v2 sum(0, 0);
	float dist = 40.0f;
	int count = 0;
	for (size_t i = 0; i < _list.size(); ++i) {
		if (i != index) {
			Dodger& d = _list[i];
			v2 diff = d.position - _list[index].position;
			if (sqr_length(diff) < (dist * dist)) {
				++count;
				sum += d.velocity;
			}
		}
	}
	if (count > 0) {
		sum.x /= count;
		sum.y /= count;
		// Implement Reynolds: Steering = Desired - Velocity
		normalize(sum);
		sum.x *= 1.2f;
		sum.y *= 1.2f;
		//sum.mult(maxspeed);
		v2 steer = sum - _list[index].velocity;
		//clamp(&sum, v2(0, 0), v2(100, 100));// steer.limit(maxforce);
		return steer;
	}
	else {
		return v2(0, 0);
	}
}

v2 Dodgers::separate(int index) {
	float desiredseparation = 25.0f;
	v2 steer(0, 0);
	float dist = 40.0f;
	int count = 0;
	// For every boid in the system, check if it's too close
	for (size_t i = 0; i < _list.size(); ++i) {
		if (i != index) {
			Dodger& d = _list[i];
			v2 diff = _list[index].position - d.position;
			if (sqr_length(diff) < (dist * dist)) {
				float d = length(diff);
				normalize(diff);
				diff.x /= d;
				diff.y /= d;
				steer += diff;
				++count;
			}
		}
	}
	// Average -- divide by how many
	if (count > 0) {
		steer.x /= count;
		steer.y /= count;
	}

	// As long as the vector is greater than 0
	if (sqr_length(steer) > 0.0f) {
		// Implement Reynolds: Steering = Desired - Velocity
		normalize(steer);
		steer.x *= 200.0f;
		steer.y *= 200.0f;
		steer -= _list[index].velocity;
	}
	return steer;
}
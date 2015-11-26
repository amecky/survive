#include "Bouncer.h"
#include "..\Constants.h"

Bouncer::Bouncer(GameContext* context, const SpawnerData& data) : ds::TimedObject(), Enemies(context,data) {
	
}

Bouncer::~Bouncer() {

}
// ------------------------------------------------
// create dodger
// ------------------------------------------------
void Bouncer::create(const StartPoint& start) {
	if (_counter < _maxEnemies) {
		Enemy d;
		d.sid = _context->world->create(start.position, "bouncer", OBJECT_LAYER);
		//_context->world->setColor(d.sid, ds::Color(255,0,0,255));
		_context->world->attachCollider(d.sid, SNAKE_TAIL, OBJECT_LAYER);
		d.lightID = _context->world->create(start.position, "lightning", LIGHT_LAYER);
		d.timer = 0.0f;
		d.position = start.position;
		v2 diff = v2(640, 360) - start.position;	
		float dt = dot(v2(640, 360), start.position);
		v2 rotated(start.normal.y, -start.normal.x);
		v2 dd = ds::math::reflect(diff, rotated);
		float angle = ds::math::getAngle(dd, V2_RIGHT);
		//LOG << "start: " << DBG_V2(start.position) << " normal: " << DBG_V2(start.normal) << " diff: " << DBG_V2(diff) << " dd: " << DBG_V2(dd) << " angle: " << RADTODEG(angle) << " dt: " << dt;
		//angle -= DEGTORAD(90.0f);
		//angle = ds::math::reflect(angle);
		//d.angle = ds::math::getTargetAngle(_context->playerPos, start);
		d.angle = angle;
		d.velocity = ds::math::getRadialVelocity(RADTODEG(angle), 200.0f);
		//d.velocity.x *= -1.0f;
		_context->world->moveBy(d.sid, d.velocity, true);
		_context->world->setColor(d.lightID, ds::Color(50, 213, 255, 255));
		_context->world->scale(d.lightID, 0.7f, 0.7f);
		_context->world->setRotation(d.sid, angle);
		_list.push_back(d);
		_context->trails->add(d.sid, 5.0f, 11);
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
// move
// ------------------------------------------------
void Bouncer::move(float dt) {
	v2 v;
	for (size_t i = 0; i < _list.size(); ++i) {
		Enemy& d = _list[i];
		_context->world->setPosition(d.lightID, _context->world->getPosition(d.sid));
	}
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

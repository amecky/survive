#include "DeathBalls.h"
#include "..\utils\util.h"
#include "..\Constants.h"

DeathBalls::DeathBalls(GameContext* context) : _context(context), _world(context->world) {
}

DeathBalls::~DeathBalls() {
}

void DeathBalls::start() {
	v2 pp = _world->getPosition(_context->playerID);
	v2 p = util::pickSpawnPoint(pp);
	ds::SID sid = _world->create(p, "death_ball",OBJECT_LAYER);
	_world->startBehavior(sid, "wiggle_death_ball");
	float a = ds::math::random(0.0f, TWO_PI);
	_world->setRotation(sid, a);
	_world->rotateBy(sid, TWO_PI, 3.0f);
}

void DeathBalls::handleEvents(const ds::ActionEventBuffer& buffer) {
	for (int i = 0; i < buffer.events.size(); ++i) {
		const ds::ActionEvent& event = buffer.events[i];
		if (_world->contains(event.sid) && event.spriteType == OT_DEATHBALL) {
			if (event.type == ds::AT_ROTATE_BY ) {
				float angle = _world->getRotation(event.sid);
				v2 v = ds::vector::getRadialVelocity(angle, 500.0f);
				_world->moveBy(event.sid, v);
			}
			else if (event.type == ds::AT_MOVE_BY) {
				// let it explode
				v2 p = _world->getPosition(event.sid);
				_context->particles->start(ENEMY_EXPLOSION, p);
				_world->remove(event.sid);
			}
		}
	}
}
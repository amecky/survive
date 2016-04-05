#include "DeathBalls.h"
#include "..\utils\util.h"
#include "..\Constants.h"

const float ANGLES[] = { DEGTORAD(90.0f), DEGTORAD(0.0f), DEGTORAD(270.0f), DEGTORAD(180.0f) };

DeathBalls::DeathBalls(GameContext* context) : _context(context), _world(context->world) {
}

DeathBalls::~DeathBalls() {
}

void DeathBalls::start() {
	v2 pp = _world->getPosition(_context->playerID);
	v2 p = util::pickSpawnPoint(pp);
	ds::SID sid = _world->create(p, "death_ball",OBJECT_LAYER);
	_world->startBehavior(sid, "wiggle_death_ball");

	float a = 0.0f;

	int side = 0;
	float delta = 2000.0f;
	if (640.0f - p.y < delta) {
		delta = 640.0f - p.y;
		side = 0;
	}
	if (1240.0f - p.x < delta) {
		delta = 1240.0f - p.x;
		side = 1;
	}
	if (p.y - 40.0f < delta) {
		delta = p.y - 40.0f;
		side = 2;
	}
	if (p.x - 40.0f < delta) {
		delta = p.x - 40.0f;
		side = 3;
	}
	a = ANGLES[side] + PI;

	_world->setRotation(sid, a);
	float angle = ds::math::randomRange(DEGTORAD(45.0f), DEGTORAD(45.0f));
	_world->rotateBy(sid, angle, 3.0f);
	ds::SID id = _world->create(p, "inner_death_ball", OBJECT_LAYER);
	_world->startBehavior(id, "death_ball_flashing");
	_world->setRotation(id, a);
	_world->rotateBy(id, angle, 3.0f);
	DeathBall* data = (DeathBall*)_world->attach_data(sid, sizeof(DeathBall),OT_DEATHBALL);
	data->innerID = id;
}

void DeathBalls::handleEvents(const ds::ActionEventBuffer& buffer) {
	for (int i = 0; i < buffer.events.size(); ++i) {
		const ds::ActionEvent& event = buffer.events[i];
		if (_world->contains(event.sid) && event.spriteType == OT_DEATHBALL) {
			if (event.type == ds::AT_ROTATE_BY ) {
				float angle = _world->getRotation(event.sid);
				v2 v = ds::vector::getRadialVelocity(angle, 500.0f);
				_world->moveBy(event.sid, v);
				DeathBall* data = (DeathBall*)_world->get_data(event.sid);
				XASSERT(data != 0, "No Deathball data found for %d",event.sid);
				_world->moveBy(data->innerID, v);
				_context->trails->add(event.sid, 4.0f, DEATHBALL_TRAIL, 20.0f);
				_world->stopAction(event.sid, ds::AT_COLOR_FLASH);
				_world->stopAction(data->innerID, ds::AT_COLOR_FLASH);
				_world->setColor(event.sid, ds::Color(255, 128, 0, 255));
				_world->setColor(data->innerID, ds::Color(255, 128, 0, 255));
			}
			else if (event.type == ds::AT_MOVE_BY) {
				// let it explode
				v2 p = _world->getPosition(event.sid);
				_context->particles->start(DEATHBALL_EXPLOSION, p);
				DeathBall* data = (DeathBall*)_world->get_data(event.sid);
				_world->remove(data->innerID);
				_world->remove(event.sid);				
			}
		}
	}
}
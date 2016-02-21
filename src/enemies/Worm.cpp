#include "Worm.h"


Worm::Worm(GameContext* context) : _context(context), _world(context->world), _head_id(ds::INVALID_SID), _positions(10) {
}


Worm::~Worm() {
}

void Worm::start(const v2& pos) {
	_head_id = _world->create(pos, "worm_head");
	v2 vel = ds::vector::getRadialVelocity(ds::math::random(0.0f, TWO_PI), 200.0f);
	_world->moveBy(_head_id, vel, true);
	_timer = 0.0f;
	_prev_pos = pos;
	_count = 0;
}

void Worm::tick(float dt) {
	if (_head_id != ds::INVALID_SID) {
		v2 pos = _world->getPosition(_head_id);
		v2 diff = pos - _prev_pos;
		if (sqr_length(diff) > (50.0f * 50.0f)) {
			if (_count < 5) {
				_world->create(_prev_pos, "worm_tail");
				++_count;
			}
			_positions.push(pos);
			_prev_pos = pos;
		}
	}
}

#include "Worm.h"


typedef void(*MoveFunc)(Vector2f&, float*, float);

void WormMove(v2& p, float* angle, float time) {
	p.x = 512.0f + 300.0f * cos(time * TWO_PI) + 120.0f * sin(-time * 4.0f * TWO_PI);
	p.y = 420.0f + 300.0f * sin(time * TWO_PI) - 128 * cos(time * 0.25f * TWO_PI);
	v2 d = p - v2(800, 450);
	*angle = ds::vector::calculateRotation(d) + DEGTORAD(90.0f);
}

Worm::Worm(GameContext* context) : _context(context), _world(context->world), _head_id(ds::INVALID_SID), _positions(10) {
}


Worm::~Worm() {
}

void Worm::start(const v2& pos) {
	_head_id = _world->create(pos, "worm_head");
	v2 vel = ds::vector::getRadialVelocity(ds::math::random(0.0f, TWO_PI), 40.0f);
	//_world->moveBy(_head_id, vel, true);
	_world->moveWith(_head_id, &WormMove, 12.0f, -1);
	_timer = 0.0f;
	_prev_pos = pos;
	_count = 0;
}

void Worm::tick(float dt) {
	if (_head_id != ds::INVALID_SID) {
		v2 pos = _world->getPosition(_head_id);
		v2 diff = pos - _prev_pos;
		if (sqr_length(diff) > (40.0f * 40.0f)) {
			if (_count < 5) {
				ds::SID sid = _world->create(_prev_pos, "worm_tail");
				++_count;
				_tails.push_back(sid);
			}
			if (_positions.size() > 30) {
				for (int j = 0; j < 30; ++j) {
					_positions[j] = _positions[j + 1];
				}
				_positions.pop_back();
			}
			_positions.push_back(pos);
			_prev_pos = pos;
		}
		if (_positions.size() > 0) {
			float angle = 0.0f;
			for (int i = 0; i < _tails.size(); ++i) {
				ds::SID id = _tails[i];
				v2 tp = _world->getPosition(id);
				v2 dd = _world->getPosition(_head_id) - tp;
				float dist = 40.0f * (i + 1);
				//float dist = length(dd);
				v2 start = _positions.back();
				float total = length(_world->getPosition(_head_id) - start);
				for (int j = _positions.size() - 2 ; j >= 0; --j) {
					v2 end = _positions[j];
					v2 d = end - start;
					float l = length(d);
					total += l;
					if (total >= dist) {
						ds::math::follow(start, tp, &angle, 1.0f, 2.0f * dt);
						float p = dist / total;
						v2 np = start + d * p * dt;
						//LOG << i << " / " << j << " = " << DBG_V2(start) << " d: " << DBG_V2(d) << " p: " << p;
						//_world->setPosition(id, tp);
						_world->setPosition(id, np);
						_world->setRotation(id, angle);
						break;
					}
					start = end;
				}
			}
		}
	}
}

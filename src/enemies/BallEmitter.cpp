#include "BallEmitter.h"
#include <math\Bitset.h>
#include "..\Constants.h"
#include "..\utils\util.h"

void RingSpawner::start(const v2& pos, int emitts, int pieces) {
	_pieces = pieces;
	_emitts = emitts;
	_step = TWO_PI / static_cast<float>(pieces);
	_emittStep = TWO_PI / static_cast<float>(emitts);
	_timer = 0.0f;
	_position = pos;
	_current = 0.0f;
	_currentEmitts = 0.0f;
	_active = true;
	_emittTimer = 0.0f;
}

void RingSpawner::tick(float dt, ds::Array<EmitterEvent>& buffer) {	
	float frequency = 5.0f;
	if (_active) {
		float dx = _position.x + cos(_timer) * 40.0f;
		float dy = _position.y + sin(_timer) * 40.0f;
		if (_timer >= _current) {
			_current += _step;
			_context->particles->start(ENEMY_TRAIL, v2(dx, dy));
		}
		if (_timer >= _currentEmitts) {
			_currentEmitts += _emittStep;
			EmitterEvent e;
			e.type = EmitterEvent::EMITT;
			e.position = v2(dx, dy);
			e.normal = ds::vector::getRadialVelocity(_timer, 1.0f);
			buffer.push_back(e);
		}
		_timer += dt * frequency;
		if (_timer >= TWO_PI) {
			EmitterEvent e;
			e.type = EmitterEvent::STOP;
			e.position = _position;
			buffer.push_back(e);
			_active = false;
		}
	}
}

void LineSpawner::start(const v2& start,const v2& end,int pieces) {
	_position = start;
	_end = end;
	_angle = ds::vector::getAngle(start, end);
	_velocity = ds::vector::getRadialVelocity(_angle, 500.0f);
	_prev = start;
	_active = true;
	_emittPos = start;
	float l = distance(start, end);
	_emittDistance = l / pieces;
	_id = _context->world->create(start, "emitter_head");
	_context->world->moveBy(_id, _velocity);
	_context->world->setRotation(_id, _angle);
	_context->trails->add(_id, 20.0f, ENEMY_TRAIL, 4.0f);
}

void LineSpawner::tick(float dt, ds::Array<EmitterEvent>& buffer) {
	if (_active) {
		_position += _velocity * dt;
		float d = distance(_position, _prev);
		if (distance(_position, _emittPos) > _emittDistance) {
			EmitterEvent e;
			e.type = EmitterEvent::EMITT;
			e.position = _emittPos;
			e.normal = ds::vector::getRadialVelocity(_angle, 1.0f);
			buffer.push_back(e);
			_emittPos = _position;
		}
		if (distance(_position, _end) < 10.0f) {
			EmitterEvent e;
			e.type = EmitterEvent::STOP;
			e.position = _position;
			buffer.push_back(e);
			_active = false;
			_context->world->remove(_id);
		}
	}
}

void CurveSpawner::start(const v2& start, const v2& end, int pieces) {
	_position = start;
	_end = end;
	util::buildSingleCurve(start, end, &_path,240.0f,false);
	_path.build();
	_prev = start;
	_active = true;
	_emittPos = start;
	float l = distance(start, end);
	_emittDistance = l / pieces;
	_timer = 0.0f;
	_id = _context->world->create(start, "emitter_head");
	_context->world->followPath(_id, &_path, _context->settings->curveEmitterTTL);
	_context->trails->add(_id, 10.0f, ENEMY_TRAIL, 8.0f);
}

void CurveSpawner::tick(float dt, ds::Array<EmitterEvent>& buffer) {
	if (_active) {
		_path.get(_timer, &_position);
		_timer += dt / _context->settings->curveEmitterTTL;
		float d = distance(_position, _prev);
		if (distance(_position, _emittPos) > _emittDistance) {
			EmitterEvent e;
			e.type = EmitterEvent::EMITT;
			e.position = _emittPos;
			e.normal = ds::vector::getRadialVelocity(0.0f, 1.0f);
			buffer.push_back(e);
			_emittPos = _position;
		}
		if (_timer >= 1.0f) {
			EmitterEvent e;
			e.type = EmitterEvent::STOP;
			e.position = _position;
			buffer.push_back(e);
			_active = false;
			_context->world->remove(_id);
		}
	}
}

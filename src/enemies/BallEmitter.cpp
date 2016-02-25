#include "BallEmitter.h"
#include <math\Bitset.h>
#include "..\Constants.h"
#include "..\utils\util.h"

void RingSpawner::start(const v2& pos) {
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
			_context->particles->start(ENEMY_TRAIL, v3(dx, dy, 0.0f));
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
}

void LineSpawner::tick(float dt, ds::Array<EmitterEvent>& buffer) {
	if (_active) {
		_position += _velocity * dt;
		float d = distance(_position, _prev);
		if (d > 5.0f) {
			_context->particles->start(ENEMY_TRAIL, _prev);
			_prev = _position;
		}
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
}

void CurveSpawner::tick(float dt, ds::Array<EmitterEvent>& buffer) {
	if (_active) {
		_path.get(_timer, &_position);
		_timer += dt * 0.25f;
		float d = distance(_position, _prev);
		if (d > 5.0f) {
			_context->particles->start(ENEMY_TRAIL, _prev);
			_prev = _position;
		}
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
		}
	}
}

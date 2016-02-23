#include "BallEmitter.h"
#include <math\Bitset.h>



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
			_context->particles->start(3, v3(dx,dy,0.0f));
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

// --------------------------------------
// edges spawner
// --------------------------------------
//
//    2
//  1   3
//    4
//
// --------------------------------------
BallEmitter::BallEmitter(const SpawnerData& data) : _data(data) , _points(0), _total(0), _index(0) {
	_total = 0;
	if (ds::bit::is_set(_data.sides, 0)) {
		_total += _data.count_y;
	}
	if (ds::bit::is_set(_data.sides, 1)) {
		_total += _data.count_x;
	}
	if (ds::bit::is_set(_data.sides, 2)) {
		_total += _data.count_y;
	}
	if (ds::bit::is_set(_data.sides, 3)) {
		_total += _data.count_x;
	}
	_points = new SpawnPoint[_total];
}

void BallEmitter::rebuild() {
	float stepX = (_data.world_size.x - _data.border.x * 2.0f) / static_cast<float>(_data.count_x - 1);
	float stepY = (_data.world_size.y - _data.border.y * 2.0f) / static_cast<float>(_data.count_y + 1);
	v2 steps(stepX, stepY);
	int cnt = 0;
	// bottom
	if (ds::bit::is_set(_data.sides, 3)) {
		for (int i = 0; i < _data.count_x; ++i) {
			SpawnPoint& sp = _points[cnt++];
			from_grid(i, 0, steps, sp.position);
			sp.normal = v2(0, 1);
		}
	}
	// top
	if (ds::bit::is_set(_data.sides, 1)) {
		for (int i = 0; i < _data.count_x; ++i) {
			SpawnPoint& sp = _points[cnt++];
			from_grid(_data.count_x - 1 - i, _data.count_y + 1, steps, sp.position);
			sp.normal = v2(0, -1);
		}
	}
	// left
	if (ds::bit::is_set(_data.sides, 0)) {
		for (int i = 0; i < _data.count_y; ++i) {
			SpawnPoint& sp = _points[cnt++];
			from_grid(0, i + 1, steps, sp.position);
			sp.normal = v2(1, 0);
		}
	}
	// right
	if (ds::bit::is_set(_data.sides, 2)) {
		for (int i = 0; i < _data.count_y; ++i) {
			SpawnPoint& sp = _points[cnt++];
			from_grid(_data.count_x - 1, _data.count_y - i, steps, sp.position);
			sp.normal = v2(-1, 0);
		}
	}
}

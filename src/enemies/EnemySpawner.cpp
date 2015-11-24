#include "EnemySpawner.h"
#include <math\Bitset.h>
// --------------------------------------
// edges spawner
// --------------------------------------
EdgesSpawner::EdgesSpawner(const SpawnerData& data) : EnemySpawner(data) {
	_total = 2 * _data.count_x + 2.0f * _data.count_y;
	_points = new v2[_total];
}

void EdgesSpawner::rebuild() {
	float stepX = (ds::renderer::getScreenWidth() - _data.border.x * 2.0f) / static_cast<float>(_data.count_x - 1);
	float stepY = (ds::renderer::getScreenHeight() - _data.border.y * 2.0f) / static_cast<float>(_data.count_y + 1);
	v2 steps(stepX, stepY);
	int cnt = 0;
	for (int i = 0; i < _data.count_x; ++i) {
		v2& sp = _points[cnt++];
		from_grid(i, 0, steps, sp);
	}
	for (int i = 0; i < _data.count_x; ++i) {
		v2& sp = _points[cnt++];
		from_grid(_data.count_x - 1 - i, _data.count_y + 1, steps, sp);
	}
	for (int i = 0; i < _data.count_y; ++i) {
		v2& sp = _points[cnt++];
		from_grid(0, i + 1, steps, sp);
	}
	for (int i = 0; i < _data.count_y; ++i) {
		v2& sp = _points[cnt++];
		from_grid(_data.count_x - 1, _data.count_y - i, steps, sp);
	}
}

// --------------------------------------
// partial edges spawner
// --------------------------------------
//
//    2
//  1   3
//    4
//
// --------------------------------------
PartialEdgesSpawner::PartialEdgesSpawner(int sides, const SpawnerData& data) : EnemySpawner(data) , _sides(sides) {
	_total = 0;
	if (ds::bit::isSet(sides,0)) {
		_total += _data.count_y;
	}
	if (ds::bit::isSet(sides, 1)) {
		_total += _data.count_x;
	}
	if (ds::bit::isSet(sides, 2)) {
		_total += _data.count_y;
	}
	if (ds::bit::isSet(sides, 3)) {
		_total += _data.count_x;
	}
	_points = new v2[_total];
}

void PartialEdgesSpawner::rebuild() {
	float stepX = (ds::renderer::getScreenWidth() - _data.border.x * 2.0f) / static_cast<float>(_data.count_x - 1);
	float stepY = (ds::renderer::getScreenHeight() - _data.border.y * 2.0f) / static_cast<float>(_data.count_y + 1);
	v2 steps(stepX, stepY);
	int cnt = 0;
	// bottom
	if (ds::bit::isSet(_sides, 3)) {
		for (int i = 0; i < _data.count_x; ++i) {
			v2& sp = _points[cnt++];
			from_grid(i, 0, steps, sp);
		}
	}
	// top
	if (ds::bit::isSet(_sides, 1)) {
		for (int i = 0; i < _data.count_x; ++i) {
			v2& sp = _points[cnt++];
			from_grid(_data.count_x - 1 - i, _data.count_y + 1, steps, sp);
		}
	}
	// left
	if (ds::bit::isSet(_sides, 0)) {
		for (int i = 0; i < _data.count_y; ++i) {
			v2& sp = _points[cnt++];
			from_grid(0, i + 1, steps, sp);
		}
	}
	// right
	if (ds::bit::isSet(_sides, 2)) {
		for (int i = 0; i < _data.count_y; ++i) {
			v2& sp = _points[cnt++];
			from_grid(_data.count_x - 1, _data.count_y - i, steps, sp);
		}
	}
}
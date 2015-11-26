#include "EnemySpawner.h"
#include <math\Bitset.h>
// --------------------------------------
// edges spawner
// --------------------------------------
//
//    2
//  1   3
//    4
//
// --------------------------------------
EdgesSpawner::EdgesSpawner(const SpawnerData& data) : EnemySpawner(data) {
	_total = 0;
	if (ds::bit::is_set(_data.sides,0)) {
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

void EdgesSpawner::rebuild() {
	float stepX = (ds::renderer::getScreenWidth() - _data.border.x * 2.0f) / static_cast<float>(_data.count_x - 1);
	float stepY = (ds::renderer::getScreenHeight() - _data.border.y * 2.0f) / static_cast<float>(_data.count_y + 1);
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
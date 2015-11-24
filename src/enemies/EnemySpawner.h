#pragma once
#include <Vector.h>
#include <math\GameMath.h>
#include <renderer\graphics.h>

struct SpawnerData {
	int count_x;
	int count_y;
	v2 border;
	v2 random_offset;

	SpawnerData() : count_x(0), count_y(0), border(0, 0), random_offset(0, 0) {}
};
// --------------------------------------
// enemy spawner
// --------------------------------------
class EnemySpawner {

public:
	EnemySpawner(const SpawnerData& data) : _data(data) , _points(0), _total(0) , _index(0) {}
	virtual ~EnemySpawner() {
		if (_points != 0) {
			delete[] _points;
		}
	}
	virtual void rebuild() = 0;
	const v2& next() {
		if ((_index + 1) > _total) {
			_index = 0;
		}
		return _points[_index++];
	}

	const v2& random() {
		int o = ds::math::random(0, _total);
		return _points[o];
	}
	const int size() const {
		return _total;
	}
	void from_grid(int x, int y, const v2& steps, v2& p) {
		p.x = _data.border.x + static_cast<float>(x)* steps.x;
		p.y = _data.border.y + static_cast<float>(y)* steps.y;
	}
protected:
	int _index;
	v2* _points;
	int _total;
	SpawnerData _data;
};

// --------------------------------------
// edges spawner
// --------------------------------------
class EdgesSpawner : public EnemySpawner {

public:
	EdgesSpawner(const SpawnerData& data);
	virtual ~EdgesSpawner() {}
	void rebuild();
};

// --------------------------------------
// edges spawner
// --------------------------------------
class PartialEdgesSpawner : public EnemySpawner {

public:
	PartialEdgesSpawner(int sides, const SpawnerData& data);
	virtual ~PartialEdgesSpawner() {}
	void rebuild();
private:
	int _sides;
};

// --------------------------------------
// edges spawner
// --------------------------------------
class OneSideSpawner : public EnemySpawner {

public:
	OneSideSpawner(const SpawnerData& data) : OneSideSpawner(data) {
		_total = _data.count_x;
		_points = new v2[_total];
	}
	virtual ~OneSideSpawner() {}

	void rebuild() {
		float stepX = (ds::renderer::getScreenWidth() - _data.border.x) / static_cast<float>(_data.count_x);
		float stepY = (ds::renderer::getScreenHeight() - _data.border.y) / static_cast<float>(_data.count_y);
		v2 steps(stepX, stepY);
		int cnt = 0;
		for (int i = 0; i < _data.count_x; ++i) {
			v2& sp = _points[cnt++];
			from_grid(i, 0, steps, sp);
		}
	}
};

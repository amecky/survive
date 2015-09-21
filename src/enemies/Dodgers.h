#pragma once
#include "..\utils\GameContext.h"
#include <utils\TimedObject.h>

struct SpawnBox {

	int x;
	int y;
};

struct Spawner {

	int current;
	SpawnBox boxes[24];
	int maxSpawns;
	int minSpawns;
	int totalSpawns;
	int count;

	Spawner() : current(-1), maxSpawns(0) , minSpawns(0) , totalSpawns(0) {
		for (int i = 0; i < 8; ++i) {
			boxes[i].x = i;
			boxes[i].y = 0;
		}
		for (int i = 8; i < 16; ++i) {
			boxes[i].x = i - 8;
			boxes[i].y = 5;
		}
		for (int i = 16; i < 20; ++i) {
			boxes[i].x = 0;
			boxes[i].y = i - 15;
		}
		for (int i = 20; i < 24; ++i) {
			boxes[i].x = 7;
			boxes[i].y = i - 19;
		}	
	}

	void reset() {
		current = ds::math::random(0, 24);
		totalSpawns = ds::math::random(minSpawns, maxSpawns);
		count = 0;
	}

	Vector2f pick() {
		if (current == -1 || count >= totalSpawns) {
			reset();
		}
		SpawnBox& box = boxes[current];
		float x = 25.0f + 12.0f + box.x * 125.0f;
		float y = 25.0f + 9.0f + box.y * 125.0f;
		x += ds::math::random(0.0f,75.0f);
		y += ds::math::random(0.0f, 75.0f);
		++count;
		return Vector2f(x, y);
	}
};

class Dodgers : public ds::TimedObject {

struct StartPoint {

	Vector2f position;
	float timer;

};

struct Dodger {
	ds::SID sid;
	ds::SID lightID;
	float angle;
	float wobble;
	float timer;
	v2 velocity;
	v2 position;
};

typedef std::vector<Dodger> DodgerList;
typedef std::vector<StartPoint> StartPoints;

public:
	Dodgers(GameContext* context);
	void tick(float dt);
	void move(float dt);
	void create(const Vector2f& start);
	void start();
	const bool contains(ds::SID sid) const;
	void remove(ds::SID sid);
	void killAll();
	bool kill(ds::SID sid);
private:	
	v2 align(int index);
	v2 separate(int index);
	v2 seek(int index, const v2& target);
	ds::SID findNearest(const Vector2f& pos, float radius, ds::SID self);
	GameContext* _context;
	DodgerList _list;
	StartPoints _startPoints;
	Spawner _spawner;
};
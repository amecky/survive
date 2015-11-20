#pragma once
#include "..\utils\GameContext.h"
#include <utils\TimedObject.h>
#include "Spawner.h"

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
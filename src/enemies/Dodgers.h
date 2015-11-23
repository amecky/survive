#pragma once
#include "..\utils\GameContext.h"
#include <utils\TimedObject.h>
#include "Spawner.h"
#include "Enemies.h"

class Dodgers : public ds::TimedObject , public Enemies {

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
	~Dodgers();

	void deactivate() {}

	const EnemyType getType() const {
		return ET_DODGERS;
	}

	void handleEvents(const ds::ActionEventBuffer& buffer) {}

	bool handleImpact(ds::SID sid);
	void tick(float dt);
	
	void create(const Vector2f& start);
	void activate(int maxEnemies);
	const bool contains(ds::SID sid) const;
	void remove(ds::SID sid);
	void killAll();
private:	
	void move(float dt);
	v2 align(int index);
	v2 separate(int index);
	v2 seek(int index, const v2& target);
	ds::SID findNearest(const Vector2f& pos, float radius, ds::SID self);
	DodgerList _list;
	StartPoints _startPoints;
	Spawner _spawner;
	int _maxEnemies;
	int _counter;
};
#pragma once
#include "..\utils\GameContext.h"
#include <utils\TimedObject.h>
#include "Spawner.h"
#include "Enemies.h"

class Bouncer : public ds::TimedObject, public Enemies {

typedef std::vector<StartPoint> StartPoints;

public:
	Bouncer(GameContext* context);
	~Bouncer();
	void deactivate() {}
	const EnemyType getType() const {
		return ET_BOUNCER;
	}
	void handleEvents(const ds::ActionEventBuffer& buffer) {}
	bool handleImpact(ds::SID sid);
	void tick(float dt);
	void create(const Vector2f& start);
	void activate(int maxEnemies);
	void killAll();
private:
	void move(float dt);
	StartPoints _startPoints;
	Spawner _spawner;
	int _maxEnemies;
	int _counter;
};

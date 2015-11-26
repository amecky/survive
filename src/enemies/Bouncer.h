#pragma once
#include "..\utils\GameContext.h"
#include <utils\TimedObject.h>
#include "EnemySpawner.h"
#include "Enemies.h"

class Bouncer : public ds::TimedObject, public Enemies {

public:
	Bouncer(GameContext* context,const SpawnerData& data);
	~Bouncer();
	void deactivate() {}
	const EnemyType getType() const {
		return ET_BOUNCER;
	}
	void handleEvents(const ds::ActionEventBuffer& buffer) {}
	bool handleImpact(ds::SID sid);
	void move(float dt);
	void create(const StartPoint& start);
	void activate(int maxEnemies);
	void killAll();
};

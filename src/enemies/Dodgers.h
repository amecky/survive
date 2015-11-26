#pragma once
#include "..\utils\GameContext.h"
#include <utils\TimedObject.h>
#include "Enemies.h"

class Dodgers : public ds::TimedObject , public Enemies {

typedef std::vector<StartPoint> StartPoints;

public:
	Dodgers(GameContext* context, const SpawnerData& data);
	~Dodgers();

	const EnemyType getType() const {
		return ET_DODGERS;
	}

	void handleEvents(const ds::ActionEventBuffer& buffer) {}

	bool handleImpact(ds::SID sid);
	void move(float dt);
	
	void create(const StartPoint& start);
	void activate(int maxEnemies);
	void killAll();
private:	
	v2 align(int index);
	v2 separate(int index);
	v2 seek(int index, const v2& target);
	ds::SID findNearest(const Vector2f& pos, float radius, ds::SID self);
};
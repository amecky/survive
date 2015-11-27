#pragma once
#include "..\utils\GameContext.h"
#include "Enemies.h"

class Snake : public Enemies {

struct SnakeTrail {

	ds::SID id;
	float distance;

};

typedef std::vector<SnakeTrail> TrailList;

public:
	Snake(GameContext* context,const SpawnerData& data);
	~Snake();

	const bool contains(ds::SID sid) const {
		return false;
	}

	void remove(ds::SID sid) {

	}

	bool handleImpact(ds::SID sid) {
		return false;
	}

	int getKillCounter() const {
		return 0;
	}

	void killAll() {}

	void move(float dt);
	void handleEvents(const ds::ActionEventBuffer& buffer);
	const EnemyType getType() const {
		return ET_SNAKE;
	}
	void create(const StartPoint& start);
private:
	void createTail(const Vector2f& start, float scale);
	ds::SID m_HeadID;
	TrailList m_Trails;
	bool m_Active;
};


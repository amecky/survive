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
	Snake(GameContext* context);
	~Snake();

	

	const bool contains(ds::SID sid) const {
		return false;
	}

	void remove(ds::SID sid) {

	}

	void handleImpact(ds::SID sid) {
	}

	

	int getKillCounter() const {
		return 0;
	}


	void deactivate();
	void activate();
	void tick(float dt);
	void handleEvents(const ds::ActionEventBuffer& buffer);
	const EnemyType getType() const {
		return ET_SNAKE;
	}

	void debug();
	const bool isActive() const {
		return m_Active;
	}
	void removeTail(ds::SID sid);
	bool containsTail(ds::SID sid);
	bool isHead(ds::SID sid) {
		return sid == m_HeadID;
	}
	const bool isKillable() const {
		return m_Kills >= m_KillSize;
	}
	bool incHeadShots() {
		++m_HeadShots;
		// FIXME: read from settings
		return m_HeadShots >= 4;
	}
private:
	void createTail(const Vector2f& start, float scale);
	ds::SID m_HeadID;
	ds::SID m_ShieldID;
	ds::CubicBezierPath m_Path;
	TrailList m_Trails;
	ds::Texture m_DotTex;
	int m_Sector;
	bool m_Active;
	int m_Kills;
	int m_KillSize;
	float m_SpawnTimer;
	bool m_ShieldActive;
	int m_HeadShots;
};


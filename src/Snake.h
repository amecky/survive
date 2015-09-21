#pragma once
#include "utils\GameContext.h"


class Snake {

struct SnakeTrail {

	ds::SID id;
	float distance;

};

typedef std::vector<SnakeTrail> TrailList;

public:
	Snake(GameContext* context);
	~Snake();
	void start(int killSize);
	void moveTrail(float dt);
	void handleEvents(const ds::ActionEventBuffer& buffer);
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
	GameContext* ctx;
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


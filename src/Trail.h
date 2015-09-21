#pragma once
#include <world\World.h>
#include <map>


struct GameContext;

class Trail {

	enum TrailEmitType {
		TET_DISTANCE,
		TET_TIME,
		TET_FRAMES
	};

	struct TrailPiece {

		Vector2f prevPosition;
		int particleSystem;
		union {
			float distance;
			int frames;
			float time;
		};
		union {
			float timer;
			int frameCounter;
		};
		int type;
		float angle;
		TrailEmitType emitType;

	};

typedef std::map<ds::SID,TrailPiece> Pieces;

public:
	Trail(GameContext* context);
	~Trail(void);
	void add(ds::SID sid,float distance,int particleSystem);
	void add(ds::SID sid,float angle,float distance,int particleSystem);
	void addFrameBased(ds::SID sid, float angle, int frames, int particleSystem);
	void addTimeBased(ds::SID sid, float angle, float ttl, int particleSystem);
	void tick(float dt);
	void remove(ds::SID sid);
private:
	GameContext* ctx;
	Pieces m_Pieces;
};


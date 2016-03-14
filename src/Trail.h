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

		ds::SID id;
		v2 prevPosition;
		int particleSystem;
		union {
			float distance;
			int frames;
			float time;
		};
		union {
			float distanceOffset;
			int framesOffset;
			float timeOffset;
		};
		union {
			float timer;
			int frameCounter;
		};
		int type;
		float angle;
		TrailEmitType emitType;
		int emitted;

	};

public:
	Trail(GameContext* context,int maxTrails);
	~Trail(void);
	void add(ds::SID sid,float distance,int particleSystem,float offset = 0.0f);
	void add(ds::SID sid,float angle,float distance,int particleSystem,float offset = 0.0f);
	void addFrameBased(ds::SID sid, float angle, int frames, int particleSystem);
	void addTimeBased(ds::SID sid, float angle, float ttl, int particleSystem);
	void tick(float dt);
	void remove(ds::SID sid);
private:
	void prepare(TrailPiece& piece, ds::SID sid, float angle, int particleSystem, TrailEmitType type);
	void kill(int index);
	int _maxTrails;
	int _currentTrails;
	GameContext* ctx;
	//Pieces m_Pieces;
	TrailPiece* _pieces;
};


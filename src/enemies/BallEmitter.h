#pragma once
#include <Vector.h>
#include <math\GameMath.h>
#include <renderer\graphics.h>
#include <lib\collection_types.h>
#include "..\utils\GameContext.h"

// --------------------------------------
// emitter event
// --------------------------------------
struct EmitterEvent {
	enum EventType {
		EMITT,STOP,EOL
	};
	EventType type;
	v2 position;
	v2 normal;
};

// --------------------------------------
// basic cube spawner
// --------------------------------------
class CubeSpawner {

public:
	CubeSpawner(GameContext* context) : _context(context) , _active(false) {}
	virtual ~CubeSpawner() {}
	virtual void tick(float dt, ds::Array<EmitterEvent>& buffer) = 0;
protected:
	GameContext* _context;
	bool _active;
};

// --------------------------------------
// ring spawner
// --------------------------------------
class RingSpawner : public CubeSpawner {

public:
	RingSpawner(GameContext* context,int emitts,int pieces) : CubeSpawner(context) , _pieces(pieces) , _emitts(emitts) {
		_step = TWO_PI / static_cast<float>(pieces);
		_emittStep = TWO_PI / static_cast<float>(emitts);
		LOG << "step: " << RADTODEG(_step) << " emitt step: " << RADTODEG(_emittStep);
	}
	virtual ~RingSpawner() {}
	void start(const v2& pos);
	void tick(float dt, ds::Array<EmitterEvent>& buffer);
private:
	int _pieces;
	v2 _position;
	float _timer;
	float _emittTimer;
	float _step;
	float _current;

	int _emitts;
	float _emittStep;
	float _currentEmitts;

};

// --------------------------------------
// line spawner
// --------------------------------------
class LineSpawner : public CubeSpawner {

public:
	LineSpawner(GameContext* context) : CubeSpawner(context) {}
	virtual ~LineSpawner() {}
	void start(const v2& start,const v2& end,int pieces);
	void tick(float dt, ds::Array<EmitterEvent>& buffer);
private:
	int _pieces;
	v2 _position;
	v2 _end;
	v2 _velocity;
	v2 _prev;
	v2 _emittPos;
	float _emittDistance;
	float _angle;
};

// --------------------------------------
// curve spawner
// --------------------------------------
class CurveSpawner : public CubeSpawner {

public:
	CurveSpawner(GameContext* context) : CubeSpawner(context) {}
	virtual ~CurveSpawner() {}
	void start(const v2& start, const v2& end, int pieces);
	void tick(float dt, ds::Array<EmitterEvent>& buffer);
private:
	int _pieces;
	v2 _position;
	v2 _end;
	ds::CubicBezierPath _path;
	float _timer;

	v2 _prev;
	v2 _emittPos;
	float _emittDistance;
};
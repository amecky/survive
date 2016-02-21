#pragma once
#include "..\utils\GameContext.h"

class Worm {

public:
	Worm(GameContext* context);
	~Worm();
	void start(const v2& pos);
	void tick(float dt);
private:
	GameContext* _context;
	ds::World* _world;
	ds::SID _head_id;
	float _timer;
	v2 _prev_pos;
	int _count;
	ds::FixedSizeStack<v2> _positions;
};


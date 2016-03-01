#pragma once
#include "..\utils\GameContext.h"

class DeathBalls {

	struct DeathBall {

		ds::SID innerID;

	};

public:
	DeathBalls(GameContext* context);
	~DeathBalls();
	void start();
	void handleEvents(const ds::ActionEventBuffer& buffer);
private:
	GameContext* _context;
	ds::World* _world;
};


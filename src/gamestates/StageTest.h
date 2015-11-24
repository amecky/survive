#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\utils\GameContext.h"
#include "..\enemies\EnemySpawner.h"

class StageTest : public ds::GameState {

public:
	StageTest(GameContext* ctx);
	virtual ~StageTest() {}
	int update(float dt);
	void render();
	void activate();
	void deactivate();
private:
	GameContext* _context;
	EnemySpawner* _spawner;
};


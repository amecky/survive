#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\utils\GameContext.h"
#include "..\enemies\EnemySpawner.h"
#include "..\enemies\Bouncer.h"
#include "..\enemies\StageManager.h"

class StageTest : public ds::GameState {

public:
	StageTest(GameContext* ctx);
	virtual ~StageTest();
	int update(float dt);
	void render();
	void activate();
	void deactivate();
private:
	GameContext* _context;
	StageManager* _stageManager;
	v2 _startPos;
	int _state;
	SpawnerData _data;
	int _count;
	std::vector<std::string> _emitterTypes;
	int _emitterSelection;
	std::vector<std::string> _enemyTypes;
	int _enemySelection;
	bool _showEditor;
};


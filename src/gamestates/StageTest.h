#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\utils\GameContext.h"
#include "..\Player.h"
#include "..\enemies\EnergyBalls.h"

class StageTest : public ds::GameState {

public:
	StageTest(GameContext* ctx);
	virtual ~StageTest();
	int update(float dt);
	void render();
	void activate();
	void deactivate();
private:
	void drawBorder(const v2& pos, const v2& center,const ds::Color& color,float descale);
	GameContext* _context;
	Player* _player;
	EnergyBalls* _balls;
	v2 _cursor_pos;
};


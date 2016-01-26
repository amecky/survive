#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\utils\GameContext.h"
#include "..\Player.h"
#include "..\enemies\Cubes.h"

class MainGameState : public ds::GameState {

public:
	MainGameState(GameContext* ctx);
	virtual ~MainGameState();
	int update(float dt);
	void render();
	void activate();
	void deactivate();
	int onButtonDown(int button, int x, int y);
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
private:
	bool handleCollisions();
	void killEnemy(ds::SID bulletID, const v2& bulletPos, ds::SID enemyID, const v2& enemyPos, int enemyType);
	void drawBorder(const v2& pos, const v2& center,const ds::Color& color,float descale);
	GameContext* _context;
	Player* _player;
	Cubes* _balls;
	v2 _cursor_pos;
	bool _dying;
	float _dying_timer;
};


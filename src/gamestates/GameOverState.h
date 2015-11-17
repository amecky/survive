#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\utils\GameContext.h"
class GameOverState : public ds::GameState {

public:
	GameOverState(GameContext* ctx,ds::DialogManager* gui);
	virtual ~GameOverState() {}
	int update(float dt);
	void render();
	void activate();
	void deactivate();
private:
	GameContext* _context;
	ds::DialogManager* _gui;
};
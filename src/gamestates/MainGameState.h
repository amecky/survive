#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\utils\GameContext.h"
#include "..\Player.h"
#include "..\enemies\Cubes.h"
#include "..\EventBuffer.h"
#include "..\Levels.h"
#include "..\enemies\Worm.h"
#include "..\enemies\DeathBalls.h"
#include "..\utils\BorderLines.h"

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
	void moveStars(const v2& target, float dt);
	void createStar(const v2& pos);
	void addStar(const v2& pos, int count,float radius = 20.0f);
	void spawn(float dt);

	GameContext* _context;
	ds::World* _world;
	Player* _player;
	Cubes* _cubes;
	v2 _cursor_pos;
	bool _dying;
	float _dying_timer;
	Levels _levels;
	EventBuffer _eventBuffer;
	Worm* _worm;
	RingSpawner* _spawner;
	LineSpawner* _lineSpawner;
	CurveSpawner* _curveSpawner;
	DeathBalls* _deathBalls;
	Border* _border;
	ds::Array<EmitterEvent> _events;
	float _spawnTimer;
	int _counter;
	int _killed;
};


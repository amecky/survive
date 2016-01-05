#pragma once
#include <Vector.h>
#include <renderer\render_types.h>
#include <world\World.h>
#include <particles\ParticleSystem.h>
#include <particles\ParticleManager.h>
#include "utils\SettingsConverter.h"
#include "utils\GameContext.h"
#include "utils\util.h"
#include <tasks\TaskManager.h>
#include "utils\IntroText.h"
#include <objects\BloomComponent.h>
#include <math\Bitset.h>
#include "Player.h"
#include <renderer\shader.h>
#include "utils\BorderLines.h"
#include <base\GameState.h>

class Trail;

class Worms : public ds::GameState {

	enum InternalState {
		IS_PREPARING,
		IS_RUNNING,
		IS_DYING
	};


public:
	Worms(GameContext* ctx);
	~Worms(void);
	void init();
	int update(float dt);
	void render();
	int onButtonDown(int button, int x, int y);
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
	void activate();
	void deactivate();
private:
	void incrementKills(int points = 100);
	void commonTick(float dt);
	void resetHUD();
	void killEnemy(ds::SID bulletID, const Vector2f& bulletPos, ds::SID enemyID, const Vector2f& enemyPos, int enemyType);
	void startShaking();
	void createStages();

	InternalState _state;
	v2 m_TargetPos;
	ds::Texture m_ColliderText;
	GameContext* _context;
	int _shakeShader;
	bool m_Shaking;
	float m_ShakeTimer;

	Player* _player;
	float _warm_up_timer;
	bool _no_enemies;

	int _viewport_id;
};


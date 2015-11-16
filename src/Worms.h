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
#include "Snake.h"
#include <objects\BloomComponent.h>
#include "enemies\Dodgers.h"
#include <math\Bitset.h>
#include "Player.h"
#include <renderer\shader.h>
#include "utils\BorderLines.h"
#include <base\GameState.h>

class Trail;

class Worms : public ds::GameState {

typedef std::vector<Snake*> Snakes;

public:
	Worms(GameContext* ctx);
	~Worms(void);
	void init();
	int update(float dt);
	void render();
	int onButtonDown(int button, int x, int y);
	int onButtonUp(int button, int x, int y);

	void debug() {
		_context->world->debug();
		_context->particles->debug();
	}
	void startShooting() {
		_player->setShooting(SM_SHOOTING);
		
	}
	void stopShooting() {
		_player->setShooting(SM_IDLE);
	}
	ds::SID pick(const Vector2f& p) {
		return _context->world->pick(p);
	}
	void debug(ds::SID sid) {
		_context->world->debug(sid);
		_context->particles->debug();
	}
	void setTargetPos(const Vector2f& tp) {
		m_TargetPos = tp;
	}
	void bomb() {
		_context->particles->start(0, Vector3f(512, 384, 0));
		_context->particles->start(2, Vector3f(512, 384, 0));
	}
	void startParticles() {
		//m_Context.particles.start(m_SelectedParticles,Vector2f(640,512));
		_context->particles->start(6, Vector3f(512, 384, 0));
	}
	void toggleDebugFlag() {
		m_DebugFlag = !m_DebugFlag;
	}
	int onChar(int ascii);
	void restart();
private:
	void incrementKills(int points = 100);
	void commonTick(float dt);
	void resetHUD();
	void killEnemy(ds::SID bulletID, const Vector2f& bulletPos, ds::SID enemyID, const Vector2f& enemyPos, int enemyType);
	void startShaking();

	//ds::Shader* _particleShader;
	//ds::Shader* _lightShader;

	Vector2f m_TargetPos;
	
	ds::Texture m_ColliderText;
	int m_AddBS;
	GameContext* _context;
	bool m_DebugFlag;
	Snakes snakes;
	int m_Level;
	ds::RTID _rt1;
	int _shakeShader;
	ds::RTID _rt2;

	bool m_Shaking;
	float m_ShakeTimer;

	ds::Bits _pbits;
	int _pindex;
	//ds::Sprite _get_ready;
	ds::SID _get_ready_id;
	Player* _player;
	Dodgers* _dodgers;
	float _warm_up_timer;
	bool _no_enemies;
	int _light_desc;
	BorderLines* _borderLines;
};


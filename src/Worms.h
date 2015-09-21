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
#include <objects\HUD.h>
#include <objects\BloomComponent.h>
#include "enemies\Dodgers.h"
#include <math\Bitset.h>
#include "Player.h"
#include <renderer\shader.h>
#include "utils\BorderLines.h"

class Trail;

class Worms {

enum GameState {
	GS_WARM_UP,
	GS_RUNNING,
	GS_DYING,
	GS_GAME_OVER
};

typedef std::vector<Snake*> Snakes;

struct HUDData {

	int points;
	int kills;

};


public:
	Worms(void);
	~Worms(void);
	void init(GameSettings* settings, ds::BitmapFont* font,ds::DialogManager* dialogs);
	void tick(float dt);
	void render();
	
	void debug() {
		m_World.debug();
		m_Context.particles.debug();
	}
	void startShooting() {
		_player->setShooting(SM_SHOOTING);
		
	}
	void stopShooting() {
		_player->setShooting(SM_IDLE);
	}
	ds::SID pick(const Vector2f& p) {
		return m_World.pick(p);
	}
	void debug(ds::SID sid) {
		m_World.debug(sid);
		m_Context.particles.debug();
	}
	void setTargetPos(const Vector2f& tp) {
		m_TargetPos = tp;
	}
	void bomb() {
		m_Context.particles.start(0,Vector3f(512,384, 0));
		m_Context.particles.start(2, Vector3f(512, 384 , 0));
	}
	void startParticles() {
		//m_Context.particles.start(m_SelectedParticles,Vector2f(640,512));
		m_Context.particles.start(6, Vector3f(512, 384, 0));
	}
	void toggleDebugFlag() {
		m_DebugFlag = !m_DebugFlag;
	}
	void OnChar(char ascii, unsigned int keyState);
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
	
	ds::World m_World;
	ds::Texture m_ColliderText;
	int m_AddBS;
	GameContext m_Context;
	bool m_DebugFlag;
	Snakes snakes;
	ds::HUD m_HUD;
	HUDData m_HudData;
	int m_Level;
	int m_RT1Tex;
	int _shakeShader;
	int m_RT2Tex;

	bool m_Shaking;
	float m_ShakeTimer;

	ds::Bits _pbits;
	int _pindex;
	//ds::Sprite _get_ready;
	ds::SID _get_ready_id;
	Player* _player;
	Dodgers* _dodgers;
	GameState _game_state;
	float _warm_up_timer;
	bool _no_enemies;
	int _light_desc;
	BorderLines* _borderLines;
};


#include "Worms.h"
#include <math\GameMath.h>
#include <sprites\SpriteBatch.h>
#include <compiler\AssetCompiler.h>
#include <renderer\Graphics.h>
#include "Constants.h"
#include "Trail.h"
#include "utils\util.h"
#include <physics\ColliderArray.h>
#include "GameRenderer.h"
#include "utils\FadingMessage.h"

Worms::Worms(GameContext* ctx) : ds::GameState("MainGameState") , _context(ctx) {
	// HERE !!!!!
	_no_enemies = false;
	_stageManager = new StageManager(ctx);
}


Worms::~Worms(void) {
	//delete m_Context.lights;
	//delete _borderLines;
	delete _stageManager;
	delete _player;
}

// --------------------------------------------------------------------------
// init
// --------------------------------------------------------------------------
void Worms::init() {
	ds::SID backID = _context->world->create(Vector2f(640, 360), "background", BG_LAYER);
	_player = new Player(_context);
	m_ColliderText = ds::math::buildTexture(0,160,40,40);

	//_shakeShader = ds::assets::loadShader("shake");
	m_ShakeTimer = 0.0f;
	m_Shaking = false;

	_context->world->setBoundingRect(ds::Rect(0, 0, 1280, 720));
	// define ignored collisions
	_context->world->ignoreCollisions(BULLET_TYPE, PLAYER_TYPE);
	_context->world->ignoreCollisions(SNAKE_HEAD, SNAKE_TAIL);
	_context->world->ignoreCollisions(SNAKE_TAIL, SNAKE_TAIL);
	_context->world->ignoreCollisions(SNAKE_HEAD, SNAKE_SHIELD);
	_context->world->ignoreCollisions(SNAKE_TAIL, SNAKE_SHIELD);
	_context->world->ignoreLayer(BG_LAYER);
	_context->world->ignoreLayer(LIGHT_LAYER);

	createStages();

}

// --------------------------------------------------------------------------
// increment kills
// --------------------------------------------------------------------------
void Worms::incrementKills(int points) {
	_context->points += points;
	++_context->kills;
	_context->hud->setNumber(0, _context->points);
	_context->hud->setNumber(1, _context->kills);
	startShaking();
}

// --------------------------------------------------------------------------
// kill enemy and bullet
// --------------------------------------------------------------------------
void Worms::killEnemy(ds::SID bulletID, const v2& bulletPos, ds::SID enemyID, const v2& enemyPos, int enemyType) {
	PR_START("Worms:killEnemy");
	int points = _stageManager->handleImpact(enemyID);
	if (points != 0) {
		incrementKills(points);
	}
	if (_context->world->contains(bulletID)) {
		_context->particles->start(1, bulletPos);
		_context->world->remove(bulletID);
	}	
	PR_END("Worms:killEnemy");
}

// --------------------------------------------------------------------------
// common tick
// --------------------------------------------------------------------------
void Worms::commonTick(float dt) {
	PR_START("Worms:commonTick");
	_context->world->tick(dt);
	_context->particles->update(dt);
	_context->trails->tick(dt);
	PR_START("Worms:commonTickAEB");
	const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();
	if (buffer.num > 0) {
		for (int i = 0; i < buffer.num; ++i) {
			if (buffer.events[i].type == ds::AT_MOVE_BY && buffer.events[i].spriteType == BULLET_TYPE) {
				_context->particles->start(1, _context->world->getPosition(buffer.events[i].sid));
				_context->world->remove(buffer.events[i].sid);
			}
		}
	}
	PR_END("Worms:commonTickAEB");
	PR_END("Worms:commonTick");
}

// --------------------------------------------------------------------------
// tick
// --------------------------------------------------------------------------
int Worms::update(float dt) {
	PR_START("Worms:tick");

	if (_state == IS_PREPARING) {
		_player->move(dt);
		_warm_up_timer += dt;
		float n = _warm_up_timer / _context->settings->warmUpTime;
		if (n >= 1.0f) {
			_stageManager->start();
			_state = IS_RUNNING;
		}
		else {
			commonTick(dt);
			_player->shootBullets(dt);
		}
	}
	if (_state == IS_DYING) {		
		commonTick(dt);
	}
	if (_state == IS_RUNNING) {

		_player->move(dt);
		
		commonTick(dt);
		
		_player->shootBullets(dt);		

		PR_START("Worms:tick:collision");
		int numCollisions = _context->world->getNumCollisions();		
		if (numCollisions > 0) {
			//LOG << "collisions: " << numCollisions;
			for (int i = 0; i < numCollisions; ++i) {
				PR_START("Worms:tick:innerCollision");
				const ds::Collision& c = _context->world->getCollision(i);				
				if (c.containsType(BULLET_TYPE)) {
					if (c.containsType(SNAKE_TAIL)) {
						if (c.firstType == BULLET_TYPE) {
							killEnemy(c.firstSID, c.firstPos, c.secondSID, c.secondPos, c.secondType);
						}
						else {
							killEnemy(c.secondSID, c.secondPos, c.firstSID, c.firstPos, c.firstType);
						}
					}
				}
				else if (c.containsType(PLAYER_TYPE)) {
					_player->kill();
					LOG << "player hit!!!!";
					_state = IS_DYING;
					_warm_up_timer = 0.0f;
					_stageManager->killAll();
					// FIXME: remove all
					//m_Context.lights->clear();
					
				}
				PR_END("Worms:tick:innerCollision");
			}
		}
		PR_END("Worms:tick:collision");
		/*
		if (m_Shaking){
			m_ShakeTimer += dt;
			if (m_ShakeTimer > m_Context.settings->shakeTTL) {
				m_Shaking = false;
			}
		}
		*/
		if (!_no_enemies) {
			_stageManager->tick(dt);
		}
	}
	if (_state == IS_DYING) {
		_warm_up_timer += dt;
		if (_warm_up_timer >= _context->settings->dyingTime) {
			return 1;
		}
	}
	PR_END("Worms:tick");
	return 0;
}

// --------------------------------------------------------------------------
// render
// --------------------------------------------------------------------------
void Worms::render() {
	_context->renderer->renderWorld();
}

// --------------------------------------------------------------------------
// deactivate
// --------------------------------------------------------------------------
void Worms::deactivate() {
	_context->hud->deactivate();
}

// --------------------------------------------------------------------------
// activate
// --------------------------------------------------------------------------
void Worms::activate() {
	_state = IS_PREPARING;
	_context->hud->activate();
	resetHUD();
	m_Shaking = false;
	// warm up
	_player->create();
	
	_warm_up_timer = 0.0f;
	
	messages::fadingMessage(_context, "get_ready", _context->settings->warmUpTime * 1.5f);

	// FIXME: clear any dodgers that are left over
	//m_Context.playerSpeed = 200.0f;
	//m_Context.doubleFire = false;
	//m_Context.fireRate = 0.75f;
	//m_Context.tripleShot = false;
}

// --------------------------------------------------------------------------
// restart HUD
// --------------------------------------------------------------------------
void Worms::resetHUD() {
	_context->points = 0;
	_context->kills = 0;
	_context->hud->setNumber(0, 0);
	_context->hud->setNumber(1, 0);
}

// --------------------------------------------------------------------------
// start shaking
// --------------------------------------------------------------------------
void Worms::startShaking() {
	if (!m_Shaking) {
		m_Shaking = true;
	}
	m_ShakeTimer = 0.0f;
}

int Worms::onButtonDown(int button, int x, int y) {
	_player->setShooting(SM_SHOOTING);
	return 0;
}

int Worms::onButtonUp(int button, int x, int y) {
	_player->setShooting(SM_IDLE);
	return 0;
}
// --------------------------------------------------------------------------
// OnChar
// --------------------------------------------------------------------------
int Worms::onChar(int ascii) {
	return 0;
}

void Worms::createStages() {
	//_stageManager->addStage(0, { 0, ET_DODGERS, 1, 20, 10, 0.0f, 1});
	//_stageManager->addStage(0, { 1, ET_DODGERS, 2, 20, 10, 0.0f, -1 });
}
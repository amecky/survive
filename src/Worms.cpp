#include "Worms.h"
#include <math\GameMath.h>
#include <sprites\SpriteBatch.h>
#include <compiler\AssetCompiler.h>
#include <renderer\Graphics.h>
#include "Constants.h"
#include "Trail.h"
#include "utils\util.h"
#include <physics\ColliderArray.h>


Worms::Worms(GameContext* ctx) : ds::GameState("MainGameState") , _context(ctx) {
	m_DebugFlag = false;
	_pbits.clear();
	_pindex = 0;

	// HERE !!!!!
	_no_enemies = false;
}


Worms::~Worms(void) {
	//delete m_Context.lights;
	delete _borderLines;
	delete _dodgers;
	delete _player;
}

// --------------------------------------------------------------------------
// init
// --------------------------------------------------------------------------
void Worms::init() {
	ds::SID backID = _context->world->create(Vector2f(512, 384), "background", BG_LAYER);
	_player = new Player(_context);
	m_AddBS = ds::renderer::createBlendState("alpha_blend_state",ds::BL_ONE, ds::BL_ONE, true);	
	m_ColliderText = ds::math::buildTexture(0,160,40,40);

	// prepare particle system
	ds::Descriptor desc;
	desc.shader = ds::shader::createParticleShader();
	assert(desc.shader != 0);
	desc.texture = 0;
	//desc.blendState = m_AddBS;
	desc.blendState = ds::renderer::getDefaultBlendState();
	_context->particles->init(desc);
	ds::assets::loadParticleSystem("particlesystems", _context->particles);

	_rt1 = ds::renderer::createRenderTarget(ds::Color(0,0,0,0));
	_rt2 = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 0));
	//snakes = new Snake(&m_Context);

	m_Level = 0;
	
	//_shakeShader = ds::assets::loadShader("shake");
	m_ShakeTimer = 0.0f;
	m_Shaking = false;

	_context->world->setBoundingRect(ds::Rect(0, 0, 1024, 768));
	// define ignored collisions
	_context->world->ignoreCollisions(BULLET_TYPE, PLAYER_TYPE);
	_context->world->ignoreCollisions(SNAKE_HEAD, SNAKE_TAIL);
	_context->world->ignoreCollisions(SNAKE_TAIL, SNAKE_TAIL);
	_context->world->ignoreCollisions(SNAKE_HEAD, SNAKE_SHIELD);
	_context->world->ignoreCollisions(SNAKE_TAIL, SNAKE_SHIELD);
	_context->world->ignoreLayer(BG_LAYER);
	_context->world->ignoreLayer(LIGHT_LAYER);

	ds::Descriptor light_desc;
	light_desc.shader = ds::renderer::loadShader("lightning", "LTTech");
	assert(light_desc.shader != 0);
	light_desc.texture = 0;
	_light_desc = ds::renderer::addDescriptor(light_desc);

	_dodgers = new Dodgers(_context);
	_borderLines = new BorderLines(_context);

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
void Worms::killEnemy(ds::SID bulletID, const Vector2f& bulletPos, ds::SID enemyID, const Vector2f& enemyPos, int enemyType) {
	
	if (enemyType == SNAKE_TAIL) {
		for (size_t i = 0; i < snakes.size(); ++i) {
			if (snakes[i]->containsTail(enemyID)) {
				snakes[i]->removeTail(enemyID);
				_context->particles->start(0, enemyPos);
				_context->particles->start(2, enemyPos);
				incrementKills(100);
			}
		}
		if (_dodgers->kill(enemyID)) {
			incrementKills(100);			
		}
	}
	if (enemyType == SNAKE_HEAD) {
		for (size_t i = 0; i < snakes.size(); ++i) {
			if (snakes[i]->isHead(enemyID) && snakes[i]->isKillable()) {
				if (snakes[i]->incHeadShots()) {
					// FIXME: kill worm
					_context->particles->start(0, enemyPos);
					_context->particles->start(6, enemyPos);
					//m_HudData.points += 1000;
					//m_HUD.setCounterValue(0, m_HudData.points);
				}
				else {
					//m_HudData.points += 1000;
					//m_HUD.setCounterValue(0, m_HudData.points);
					startShaking();
				}
			}
		}
	}
	if (_context->world->contains(bulletID)) {
		_context->particles->start(1, bulletPos);
		_context->world->remove(bulletID);
	}	
}

// --------------------------------------------------------------------------
// common tick
// --------------------------------------------------------------------------
void Worms::commonTick(float dt) {
	_context->world->tick(dt);
	_context->particles->update(dt);
	_context->trails->tick(dt);
	const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();
	if (buffer.num > 0) {
		for (int i = 0; i < buffer.num; ++i) {
			if (buffer.events[i].type == ds::AT_MOVE_BY && buffer.events[i].spriteType == BULLET_TYPE) {
				_context->particles->start(1, _context->world->getPosition(buffer.events[i].sid));
				_context->world->remove(buffer.events[i].sid);
			}
		}
	}
}

// --------------------------------------------------------------------------
// tick
// --------------------------------------------------------------------------
int Worms::update(float dt) {
	PR_START("CatcheMe:tick")

	commonTick(dt);
	
	if (_state == IS_PREPARING) {
		_player->move(dt);
		_warm_up_timer += dt;
		float n = _warm_up_timer / _context->settings->warmUpTime;
		if (n >= 1.0f) {
			_dodgers->start();
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

		for (size_t i = 0; i < snakes.size(); ++i) {
			snakes[i]->moveTrail(dt);
		}

		
		const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();
		for (size_t i = 0; i < snakes.size(); ++i) {
			snakes[i]->handleEvents(buffer);
		}

		_player->shootBullets(dt);

		PR_START("CatchMe:tick:collision")
		int numCollisions = _context->world->getNumCollisions();
		if (numCollisions > 0) {
			for (int i = 0; i < numCollisions; ++i) {
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
					_dodgers->killAll();
					// FIXME: remove all
					//m_Context.lights->clear();
					
				}
			}
		}
		PR_END("CatchMe:tick:collision")
			/*
		if (m_Shaking){
			m_ShakeTimer += dt;
			if (m_ShakeTimer > m_Context.settings->shakeTTL) {
				m_Shaking = false;
			}
		}
		*/
		if (!_no_enemies) {
			_dodgers->tick(dt);
			_dodgers->move(dt);
		}
	}
	if (_state == IS_DYING) {
		_warm_up_timer += dt;
		if (_warm_up_timer >= _context->settings->dyingTime) {
			return 1;
		}
	}
	PR_END("CatcheMe:tick")
	return 0;
}

// --------------------------------------------------------------------------
// render
// --------------------------------------------------------------------------
void Worms::render() {
	PR_START("CatchMe:render")
	ds::renderer::setRenderTarget(_rt1);
	_context->world->renderSingleLayer(BG_LAYER);
	ds::sprites::flush();
	int current = ds::sprites::getDescriptorID();
	ds::sprites::setDescriptorID(_light_desc);
	_context->world->renderSingleLayer(LIGHT_LAYER);
	ds::sprites::flush();
	ds::sprites::setDescriptorID(current);
	_context->world->renderSingleLayer(MESSAGE_LAYER);
	_context->particles->render();
	_context->world->renderSingleLayer(OBJECT_LAYER);
	ds::renderer::restoreBackBuffer();
	ds::renderer::draw_render_target(_rt1);
	PR_END("CatchMe:render")
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
	
	// create light behind get ready message
	_get_ready_id = _context->world->create(Vector2f(512, 384), ds::math::buildTexture(600, 512, 440, 102), -1, LIGHT_LAYER);
	_context->world->setColor(_get_ready_id, ds::Color(192, 0, 0, 255));
	_context->world->removeAfter(_get_ready_id, _context->settings->warmUpTime);
	_context->world->fadeAlphaTo(_get_ready_id, 1.0f, 0.0f, _context->settings->warmUpTime);

	// create get ready message
	ds::SID gid = _context->world->create(Vector2f(512, 384), "get_ready", MESSAGE_LAYER);
	_context->world->removeAfter(gid, _context->settings->warmUpTime);
	_context->world->fadeAlphaTo(gid, 1.0f, 0.0f, _context->settings->warmUpTime);

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
	if (ascii == '3') {
		_dodgers->start();
	}
	return 0;
}


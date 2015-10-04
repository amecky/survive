#include "Worms.h"
#include <math\GameMath.h>
#include <sprites\SpriteBatch.h>
#include <compiler\AssetCompiler.h>
#include <renderer\Graphics.h>
#include "Constants.h"
#include "Trail.h"
#include "utils\util.h"
#include <physics\ColliderArray.h>


Worms::Worms(void) {
	m_DebugFlag = false;
	_pbits.clear();
	_pindex = 0;

	// HERE !!!!!
	_no_enemies = true;
}


Worms::~Worms(void) {
	delete _borderLines;
	delete m_Context.trails;
	//delete m_Context.lights;
	delete _dodgers;
	delete _player;
}

// --------------------------------------------------------------------------
// init
// --------------------------------------------------------------------------
void Worms::init(GameSettings* settings, ds::BitmapFont* font,ds::DialogManager* dialogs) {

	m_Context.world = &m_World;
	m_HUD.init(0, "xscale");
	ds::assets::load("hud", &m_HUD, ds::CVT_HUD);
	resetHUD();
	

	ds::assets::loadSpriteTemplates("sprites");
	m_Context.settings = settings;
	m_Context.font = font;
	m_Context.dialogs = dialogs;

	ds::SID backID = m_World.create(Vector2f(512, 384), "background", BG_LAYER);

	_player = new Player(&m_Context);
	m_AddBS = ds::renderer::createBlendState("alpha_blend_state",ds::BL_ONE, ds::BL_ONE, true);	
	m_ColliderText = ds::math::buildTexture(0,160,40,40);

	// prepare particle system
	ds::Descriptor desc;
	//desc.shader = ds::renderer::loadShader("particle_shader", "ParticleTech");
	desc.shader = ds::assets::loadShader("particles");// "particle_shader", "ParticleTech");
	assert(desc.shader != 0);
	desc.texture = 0;
	desc.blendState = m_AddBS;
	//desc.blendState = ds::renderer::getDefaultBlendState();
	m_Context.particles.init(desc);
	ds::assets::loadParticleSystem("particlesystems", &m_Context.particles);

	m_Context.trails = new Trail(&m_Context);
	
	m_RT1Tex = ds::renderer::createRenderTarget(0, ds::Color(0,0,0,0));
	m_RT2Tex = ds::renderer::createRenderTarget(1, ds::Color(0, 0, 0, 0));
	//snakes = new Snake(&m_Context);

	m_Level = 0;
	
	_shakeShader = ds::assets::loadShader("shake");
	m_ShakeTimer = 0.0f;
	m_Shaking = false;

	m_World.setBoundingRect(ds::Rect(0, 0, 1024, 768));
	// define ignored collisions
	m_World.ignoreCollisions(BULLET_TYPE, PLAYER_TYPE);
	m_World.ignoreCollisions(SNAKE_HEAD, SNAKE_TAIL);
	m_World.ignoreCollisions(SNAKE_TAIL, SNAKE_TAIL);
	m_World.ignoreCollisions(SNAKE_HEAD, SNAKE_SHIELD);
	m_World.ignoreCollisions(SNAKE_TAIL, SNAKE_SHIELD);
	m_World.ignoreLayer(BG_LAYER);
	m_World.ignoreLayer(LIGHT_LAYER);

	ds::Descriptor light_desc;
	light_desc.shader = ds::assets::loadShader("lightning");// ds::renderer::loadShader("lightning", "LTTech");
	assert(light_desc.shader != 0);
	light_desc.texture = 0;
	_light_desc = ds::renderer::addDescriptor(light_desc);

	_dodgers = new Dodgers(&m_Context);
	_borderLines = new BorderLines(&m_Context);

	restart();
}

void Worms::incrementKills(int points) {
	m_HudData.points += points;
	++m_HudData.kills;
	m_HUD.setCounterValue(0, m_HudData.points);
	m_HUD.setCounterValue(1, m_HudData.kills);
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
				m_Context.particles.start(0, enemyPos);
				m_Context.particles.start(2, enemyPos);
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
					m_Context.particles.start(0, enemyPos);
					m_Context.particles.start(6, enemyPos);
					m_HudData.points += 1000;
					m_HUD.setCounterValue(0, m_HudData.points);
				}
				else {
					m_HudData.points += 1000;
					m_HUD.setCounterValue(0, m_HudData.points);
					startShaking();
				}
			}
		}
	}
	if (m_World.contains(bulletID)) {
		m_Context.particles.start(1, bulletPos);
		m_World.remove(bulletID);
	}	
}

void Worms::commonTick(float dt) {
	m_World.tick(dt);
	m_Context.particles.update(dt);
	m_Context.trails->tick(dt);
	const ds::ActionEventBuffer& buffer = m_World.getEventBuffer();
	if (buffer.num > 0) {
		for (int i = 0; i < buffer.num; ++i) {
			if (buffer.events[i].type == ds::AT_MOVE_BY && buffer.events[i].spriteType == BULLET_TYPE) {
				m_Context.particles.start(1, m_World.getPosition(buffer.events[i].sid));
				m_World.remove(buffer.events[i].sid);
			}
		}
	}
}
// --------------------------------------------------------------------------
// tick
// --------------------------------------------------------------------------
void Worms::tick(float dt) {
	PR_START("CatcheMe:tick")

	_borderLines->update(dt);

	if (_game_state == GS_WARM_UP) {
		_player->move(dt);
		_warm_up_timer += dt;
		float n = _warm_up_timer / m_Context.settings->warmUpTime;
		if (n >= 1.0f) {
			_dodgers->start();
			_game_state = GS_RUNNING;
		}
		else {
			commonTick(dt);
			_player->shootBullets(dt);
		}
	}
	if (_game_state == GS_DYING) {		
		commonTick(dt);
	}
	if (_game_state == GS_RUNNING) {

		_player->move(dt);
		
		commonTick(dt);

		for (size_t i = 0; i < snakes.size(); ++i) {
			snakes[i]->moveTrail(dt);
		}

		
		const ds::ActionEventBuffer& buffer = m_World.getEventBuffer();
		for (size_t i = 0; i < snakes.size(); ++i) {
			snakes[i]->handleEvents(buffer);
		}

		_player->shootBullets(dt);

		PR_START("CatchMe:tick:collision")
		int numCollisions = m_World.getNumCollisions();
		if (numCollisions > 0) {
			for (int i = 0; i < numCollisions; ++i) {
				const ds::Collision& c = m_World.getCollision(i);
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
					_game_state = GS_DYING;
					_warm_up_timer = 0.0f;
					_dodgers->killAll();
					// FIXME: remove all
					//m_Context.lights->clear();
					
				}
			}
		}
		PR_END("CatchMe:tick:collision")
		
		if (m_Shaking){
			m_ShakeTimer += dt;
			if (m_ShakeTimer > m_Context.settings->shakeTTL) {
				m_Shaking = false;
			}
		}
		if (!_no_enemies) {
			_dodgers->tick(dt);
			_dodgers->move(dt);
		}
	}
	if (_game_state == GS_DYING) {
		_warm_up_timer += dt;
		if (_warm_up_timer >= m_Context.settings->dyingTime) {
			m_Context.dialogs->activate("GameOver");
			_game_state = GS_GAME_OVER;
			ds::GUIDialog* dlg = m_Context.dialogs->get("GameOver");
			std::string str;
			ds::string::formatInt(m_HudData.points, str);
			dlg->updateText(12, str);
			ds::string::formatInt(m_HudData.kills, str);
			dlg->updateText(8, str);
		}
	}
	PR_END("CatcheMe:tick")
}

// --------------------------------------------------------------------------
// render
// --------------------------------------------------------------------------
void Worms::render() {
	PR_START("CatchMe:render")
	

	if (_game_state == GS_RUNNING || _game_state == GS_WARM_UP || _game_state == GS_DYING) {
		
		_borderLines->draw();

		int sid = ds::renderer::getDefaultShaderID();
		if (m_Shaking) {
			ds::Shader* shader = ds::renderer::getShader(_shakeShader);
			float norm = m_ShakeTimer;
			shader->setFloat("timer", norm);
			shader->setFloat("period", m_Context.settings->shakePeriod);
			shader->setFloat("shakeAmount", m_Context.settings->shakeAmount / 1024.0f);
			ds::renderer::setCurrentShader(_shakeShader);
			sid = _shakeShader;
		}
		
		ds::renderer::setRenderTarget(0);
		
		m_World.renderSingleLayer(BG_LAYER);
		
		
		//m_Context.lights->render();

		ds::sprites::flush();
		int current = ds::sprites::getDescriptorID();
		ds::sprites::setDescriptorID(_light_desc);

		m_World.renderSingleLayer(LIGHT_LAYER);

		ds::sprites::flush();
		ds::sprites::setDescriptorID(current);

		m_World.renderSingleLayer(MESSAGE_LAYER);

		m_Context.particles.render();
		
		m_World.renderSingleLayer(OBJECT_LAYER);
		
		ds::renderer::restoreBackBuffer();
		ds::renderer::draw_render_target(0, sid);

		if (m_DebugFlag) {
			//_snakes[i]->debug();
		}

		m_HUD.render();
	}

	//m_World.drawColliders(m_ColliderText);
	//snakes->debug();
	PR_END("CatchMe:render")
}

// --------------------------------------------------------------------------
// restart game
// --------------------------------------------------------------------------
void Worms::restart() {
	resetHUD();
	m_Shaking = false;
	// warm up
	_game_state = GS_WARM_UP;
	_player->create();
	
	_warm_up_timer = 0.0f;
	
	// create light behind get ready message
	_get_ready_id = m_World.create(Vector2f(512, 384), ds::math::buildTexture(600, 512, 440, 102), -1, LIGHT_LAYER);
	m_World.setColor(_get_ready_id, ds::Color(192, 0, 0, 255));
	m_World.removeAfter(_get_ready_id, m_Context.settings->warmUpTime);
	m_World.fadeAlphaTo(_get_ready_id, 1.0f, 0.0f, m_Context.settings->warmUpTime);

	// create get ready message
	ds::SID gid = m_World.create(Vector2f(512, 384), "get_ready", MESSAGE_LAYER);
	m_World.removeAfter(gid, m_Context.settings->warmUpTime);
	m_World.fadeAlphaTo(gid, 1.0f, 0.0f, m_Context.settings->warmUpTime);

	// FIXME: clear any dodgers that are left over
	m_Context.playerSpeed = 200.0f;
	m_Context.doubleFire = false;
	m_Context.fireRate = 0.75f;
	m_Context.tripleShot = false;
}

// --------------------------------------------------------------------------
// restart HUD
// --------------------------------------------------------------------------
void Worms::resetHUD() {
	m_HudData.points = 0;
	m_HudData.kills = 0;
	m_HUD.setCounterValue(0, 0);
	m_HUD.setCounterValue(1, 0);
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

// --------------------------------------------------------------------------
// OnChar
// --------------------------------------------------------------------------
void Worms::OnChar(char ascii, unsigned int keyState) {
	if (ascii == '1') {
	}
	if (ascii == '2') {
		Snake* s = new Snake(&m_Context);
		s->start(20);
		m_HudData.kills = 0;
		snakes.push_back(s);
	}
	if (ascii == '3') {
		m_World.debug();
	}
	if (ascii == '4') {
		startShaking();
	}
	if (ascii == '6') {
		_dodgers->start();
	}
	if (ascii == 'r') {
		restart();
	}
	if (ascii == '7') {
		Vector2f mp = ds::renderer::getMousePosition();
		
		//int idx = m_Context.lights->add(mp, ds::Color(255, 0, 0, 255), 0.5f, 4);
		//m_Context.lights->scale(idx, { { 0.2f, 0.2f }, { 0.8f, 0.8f } });
	}
	if (ascii == 'i') {
		for (int i = 0; i < 16; ++i) {
			if (_pbits.isSet(i) != 0) {
				m_Context.particles.start(i, Vector3f(512, 384, 0));
			}
		}
	}
	if (ascii == '+') {
		++_pindex;
		if (_pindex > 15) {
			_pindex = 0;
		}
		LOG << "pindex: " << _pindex;
	}
	if (ascii == '-') {
		--_pindex;
		if (_pindex < 0) {
			_pindex = 15;
		}
		LOG << "pindex: " << _pindex;
	}
	if (ascii == '.') {
		_pbits.toggle(_pindex);		
		char buffer[256];
		std::string tmp;
		for (int i = 0; i < 16; ++i) {
			if (_pbits.isSet(i)) {
				sprintf(buffer, "x ");
			}
			else {
				sprintf(buffer, "- ");
			}			
			tmp += buffer;
		}
		LOG << tmp;
	}
}


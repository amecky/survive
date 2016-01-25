#include "MainGameState.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>
#include <utils\Profiler.h>
#include "..\GameRenderer.h"
#include "..\Constants.h"

MainGameState::MainGameState(GameContext* ctx) : ds::GameState("MainGameState"), _context(ctx) {
	_player = new Player(_context);
	
	_balls = new Cubes(_context);
	_context->world->setBoundingRect(ds::Rect(40, 40, 1520, 820));
	//_context->world->create(v2(800, 450), ds::math::buildTexture(840, 360, 120, 120), OBJECT_LAYER);
	_context->world->ignoreCollisions(OT_PLAYER, OT_BULLET);
	_context->world->ignoreCollisions(OT_FOLLOWER, OT_FOLLOWER);
	_context->world->ignoreCollisions(OT_FOLLOWER, OT_BIG_CUBE);
	_context->world->ignoreCollisions(OT_FOLLOWER, OT_HUGE_CUBE);
	_context->world->ignoreCollisions(OT_BIG_CUBE, OT_BIG_CUBE);
	_context->world->ignoreCollisions(OT_BIG_CUBE, OT_HUGE_CUBE);
	_context->world->ignoreCollisions(OT_HUGE_CUBE, OT_HUGE_CUBE);
	//_context->world->ignoreLayer(LIGHT_LAYER);
}

MainGameState::~MainGameState() {
	delete _balls;
	delete _player;
}

// --------------------------------------------------------------------------
// kill enemy and bullet
// --------------------------------------------------------------------------
void MainGameState::killEnemy(ds::SID bulletID, const v2& bulletPos, ds::SID enemyID, const v2& enemyPos, int enemyType) {
	ZoneTracker z("MainGameState::killEnemy");
	//PR_START("Worms:killEnemy");
	if (_context->world->contains(bulletID)) {
		_context->particles->start(BULLET_EXPLOSION, bulletPos);
		_context->world->remove(bulletID);
	}
	int type = _balls->kill(enemyID);
	if (type >= 0) {
		_context->particles->start(ENEMY_EXPLOSION, enemyPos);
	}
	//PR_END("Worms:killEnemy");
}

bool MainGameState::handleCollisions() {
	ZoneTracker z("MainGameState:tick:collision");
	bool ret = false;
	int numCollisions = _context->world->getNumCollisions();
	if (numCollisions > 0) {
		TIMER("HandleCollisions")
		//LOG << "collisions: " << numCollisions;
		for (int i = 0; i < numCollisions; ++i) {
			ZoneTracker z1("MainGameState:tick:innerCollision");
			const ds::Collision& c = _context->world->getCollision(i);
			//LOG << i << " first: " << c.firstType << " second: " << c.secondType;
			if (c.containsType(OT_BULLET)) {
				//if (c.containsType(ENEMY_TYPE)) {
					if (c.firstType == OT_BULLET) {
						killEnemy(c.firstSID, c.firstPos, c.secondSID, c.secondPos, c.secondType);
					}
					else {
						killEnemy(c.secondSID, c.secondPos, c.firstSID, c.firstPos, c.firstType);
					}
				//}
			}
			else if (c.containsType(OT_PLAYER)) {
				
				_player->kill();
				//LOG << "player hit!!!!";
				//_state = IS_DYING;
				//_warm_up_timer = 0.0f;
				// FIXME: remove all
				//m_Context.lights->clear();
				_balls->killAll();		
				ret = true;								
			}
			//PR_END("MainGameState:tick:innerCollision");
		}
	}
	//PR_END("MainGameState:tick:collision");
	return ret;
}
// -------------------------------------------------------
// update
// -------------------------------------------------------
int MainGameState::update(float dt) {
	ZoneTracker z("MainGameState:update");	
	if (!_dying) {
		ZoneTracker z2("MainGameState:update:move");
		_cursor_pos = ds::renderer::getMousePosition();
		_player->move(dt);
		_player->shootBullets(dt);

		_balls->move(dt);
		_balls->spawn(dt);
	}
	//_context->playerPos = ds::renderer::getMousePosition();
	//_context->world->setPosition(_context->playerID, _context->playerPos);
	_context->world->tick(dt);
	//PR_END("MainGameState:update:move");
	

	_context->particles->update(dt);
	//_context->trails->tick(dt);
	if (!_dying) {
		ZoneTracker z3("MainGameState:commonTickAEB");
		const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();

		_balls->handleEvents(buffer);

		if (buffer.events.size() > 0) {
			for (int i = 0; i < buffer.events.size(); ++i) {
				if (buffer.events[i].type == ds::AT_MOVE_BY && buffer.events[i].spriteType == OT_BULLET) {
					_context->particles->start(BULLET_EXPLOSION, _context->world->getPosition(buffer.events[i].sid));
					_context->world->remove(buffer.events[i].sid);
				}
			}
		}
		//PR_END("MainGameState:commonTickAEB");

		//_snake->tick(dt);
		//_snake->handleEvents(buffer);
		//_snake->move(dt);		
		if (handleCollisions()) {
			_dying = true;
		}
	}
	if (_dying) {
		_dying_timer -= dt;
		if (_dying_timer <= 0.0f) {
			return 1;
		}
	}
	//PR_END("MainGameState:update");
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {

	_context->renderer->renderWorld();
	//ds::renderer::selectViewport(_context->viewport_id);
	//_balls->render();
	ds::renderer::selectViewport(0);
	ds::sprites::draw(_cursor_pos, ds::math::buildTexture(40, 160, 20, 20));
}

void MainGameState::drawBorder(const v2& pos, const v2& center, const ds::Color& color, float descale) {
	v2 p;
	p.x = pos.x - center.x;
	p.y = pos.y - center.y;
	ds::sprites::draw(p, ds::math::buildTexture(720, 640, 320, 180), 0.0f, 4.0f - descale, 4.0f - descale * 1.5f, color);
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	_balls->activate();
	_player->create();
	_player->setShooting(SM_IDLE);
	_dying = false;
	_dying_timer = 4.0f;
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
}

int MainGameState::onButtonDown(int button, int x, int y) {
	_player->setShooting(SM_SHOOTING);
	return 0;
}

int MainGameState::onButtonUp(int button, int x, int y) {
	_player->setShooting(SM_IDLE);
	return 0;
}

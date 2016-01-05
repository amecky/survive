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
	
	_balls = new EnergyBalls(_context);
	_context->world->setBoundingRect(ds::Rect(40, 40, 1520, 820));
	_context->world->create(v2(800, 450), ds::math::buildTexture(840, 360, 120, 120), OBJECT_LAYER);
	_context->world->ignoreCollisions(BULLET_TYPE, PLAYER_TYPE);
	_context->world->ignoreCollisions(ENEMY_TYPE, ENEMY_TYPE);
}

MainGameState::~MainGameState() {
	delete _balls;
	delete _player;
}

// --------------------------------------------------------------------------
// kill enemy and bullet
// --------------------------------------------------------------------------
void MainGameState::killEnemy(ds::SID bulletID, const v2& bulletPos, ds::SID enemyID, const v2& enemyPos, int enemyType) {
	PR_START("Worms:killEnemy");
	if (_context->world->contains(bulletID)) {
		_context->particles->start(BULLET_EXPLOSION, bulletPos);
		_context->world->remove(bulletID);
	}
	int type = _balls->kill(enemyID);
	if (type >= 0) {
		_context->particles->start(ENEMY_EXPLOSION, enemyPos);
	}
	PR_END("Worms:killEnemy");
}

bool MainGameState::handleCollisions() {
	PR_START("MainGameState:tick:collision");
	int numCollisions = _context->world->getNumCollisions();
	if (numCollisions > 0) {
		//LOG << "collisions: " << numCollisions;
		for (int i = 0; i < numCollisions; ++i) {
			PR_START("MainGameState:tick:innerCollision");
			const ds::Collision& c = _context->world->getCollision(i);
			if (c.containsType(BULLET_TYPE)) {
				//if (c.containsType(ENEMY_TYPE)) {
					if (c.firstType == BULLET_TYPE) {
						killEnemy(c.firstSID, c.firstPos, c.secondSID, c.secondPos, c.secondType);
					}
					else {
						killEnemy(c.secondSID, c.secondPos, c.firstSID, c.firstPos, c.firstType);
					}
				//}
			}
			else if (c.containsType(PLAYER_TYPE)) {
				_player->kill();
				LOG << "player hit!!!!";
				//_state = IS_DYING;
				//_warm_up_timer = 0.0f;
				// FIXME: remove all
				//m_Context.lights->clear();
				_balls->killAll();
				return true;
			}
			PR_END("MainGameState:tick:innerCollision");
		}
	}
	PR_END("MainGameState:tick:collision");
	return false;
}
// -------------------------------------------------------
// update
// -------------------------------------------------------
int MainGameState::update(float dt) {
	_cursor_pos = ds::renderer::getMousePosition();
	_player->move(dt);
	_player->shootBullets(dt);
	_balls->tick(dt);
	//_context->playerPos = ds::renderer::getMousePosition();
	//_context->world->setPosition(_context->playerID, _context->playerPos);
	_context->world->tick(dt);

	if (handleCollisions()) {
		return 1;
	}

	_context->particles->update(dt);
	_context->trails->tick(dt);

	PR_START("MainGameState:commonTickAEB");
	const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();
	if (buffer.num > 0) {
		for (int i = 0; i < buffer.num; ++i) {
			if (buffer.events[i].type == ds::AT_MOVE_BY && buffer.events[i].spriteType == BULLET_TYPE) {
				_context->particles->start(BULLET_EXPLOSION, _context->world->getPosition(buffer.events[i].sid));
				_context->world->remove(buffer.events[i].sid);
			}
		}
	}
	PR_END("MainGameState:commonTickAEB");

	//_snake->tick(dt);
	//_snake->handleEvents(buffer);
	//_snake->move(dt);
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

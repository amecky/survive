#include "StageTest.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>
#include <utils\Profiler.h>
#include "..\GameRenderer.h"
#include "..\Constants.h"

StageTest::StageTest(GameContext* ctx) : ds::GameState("StageTest"), _context(ctx) {
	_player = new Player(_context);
	_player->create();
	_player->setShooting(SM_IDLE);
	_balls = new EnergyBalls(_context);

	_context->world->create(v2(800, 450), ds::math::buildTexture(840, 360, 120, 120), OBJECT_LAYER);
}

StageTest::~StageTest() {	
	delete _balls;
	delete _player;
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int StageTest::update(float dt) {
	_cursor_pos = ds::renderer::getMousePosition();
	_player->move(dt);
	_player->shootBullets(dt);

	_balls->tick(dt);
	//_context->playerPos = ds::renderer::getMousePosition();
	//_context->world->setPosition(_context->playerID, _context->playerPos);
	_context->world->tick(dt);
	_context->particles->update(dt);
	_context->trails->tick(dt);
	const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();

	//_snake->tick(dt);
	//_snake->handleEvents(buffer);
	//_snake->move(dt);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void StageTest::render() {

	_context->renderer->renderWorld();
	//ds::renderer::selectViewport(_context->viewport_id);
	//_balls->render();
	ds::renderer::selectViewport(0);
	ds::sprites::draw(_cursor_pos, ds::math::buildTexture(40, 160, 20, 20));
}

void StageTest::drawBorder(const v2& pos, const v2& center, const ds::Color& color,float descale) {
	v2 p;
	p.x = pos.x - center.x;
	p.y = pos.y - center.y;
	ds::sprites::draw(p, ds::math::buildTexture(720, 640, 320, 180), 0.0f, 4.0f - descale, 4.0f - descale * 1.5f, color);
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void StageTest::activate() {
	_balls->activate();
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void StageTest::deactivate() {
}


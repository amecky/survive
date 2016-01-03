#include "StageTest.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>
#include <utils\Profiler.h>
#include "..\GameRenderer.h"
#include "..\Constants.h"

StageTest::StageTest(GameContext* ctx) : ds::GameState("StageTest"), _context(ctx) {
	_count = 1;
	_data.count_x = 10;
	_data.count_y = 5;
	_data.border = v2(40, 40);
	_data.sides = 10;
	_data.delay = _context->settings->dodgersSpawnTimer;
	_data.type = SPT_PARTIAL_EDGES;
	_data.emitter_type = SET_IMMEDIATE;
	_stageManager = std::make_unique<StageManager>(_context);
	_startPos = v2(1000, 710);
	_state = 1;
	_emitterTypes.push_back("IMMEDIATE");
	_emitterTypes.push_back("DELAYED");
	_enemyTypes.push_back("Dodgers");
	_enemyTypes.push_back("Bouncer");
	_enemyTypes.push_back("Snake");
	_showEditor = false;
	_enemySelection = 2;
	_emitterSelection = 0;
	//_context->playerID = _context->world->create(v2(640, 360), "player",OBJECT_LAYER);
	_snake = new Snake(_context,_data);
	_player = new Player(_context);
	_player->create();
}

StageTest::~StageTest() {	
	delete _player;
	delete _snake;
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int StageTest::update(float dt) {
	_player->move(dt);
	//_context->playerPos = ds::renderer::getMousePosition();
	//_context->world->setPosition(_context->playerID, _context->playerPos);
	_context->world->tick(dt);
	_context->particles->update(dt);
	_context->trails->tick(dt);
	const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();
	_stageManager->tick(dt);
	_stageManager->handleEvents(buffer);

	_snake->tick(dt);
	_snake->handleEvents(buffer);
	//_snake->move(dt);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void StageTest::render() {

	_context->renderer->renderWorld();

	if (_showEditor) {
		int offset = 0;
		gui::start(1, &_startPos);
		if (gui::begin("Wave", &_state)) {
			gui::InputInt("Enemies", &_count);
			gui::InputInt("Count X", &_data.count_x);
			gui::InputInt("Count Y", &_data.count_y);
			gui::InputVec2("Border", &_data.border);
			gui::InputInt("Sides", &_data.sides);
			gui::InputFloat("Delay", &_data.delay);
			gui::ComboBox(_emitterTypes, &_emitterSelection, &offset, 2);
			gui::ComboBox(_enemyTypes, &_enemySelection, &offset, 3);
			gui::beginGroup();
			if (gui::Button("Kill all")) {
				_stageManager->killAll();
			}
			if (gui::Button("Start")) {
				if (_emitterSelection != -1) {
					if (_emitterSelection == 0) {
						_data.emitter_type = SET_IMMEDIATE;
					}
					else {
						_data.emitter_type = SET_DELAYED;
					}
				}
				WaveDefinition def;
				def.count = _count;
				def.enemy_type = ET_BOUNCER;
				if (_enemySelection != -1) {
					if (_enemySelection == 0) {
						def.enemy_type = ET_DODGERS;
					}
					else if (_enemySelection == 2) {
						def.enemy_type = ET_SNAKE;
					}
				}
				def.health = 1;
				def.spawner_data = _data;
				_stageManager->startWave(def);
			}
			gui::endGroup();
		}
		gui::end();
	}
	/*
	v2 diff = v2(640, 360) - _context->playerPos;
	diff *= 0.03f;
	drawBorder(v2(640, 360), diff, ds::Color(255, 120, 0, 255),0.1f);
	diff = v2(640, 360) - _context->playerPos;
	diff *= 0.025f;
	drawBorder(v2(640, 360), diff, ds::Color(192, 90, 0, 255), 0.2f);
	diff = v2(640, 360) - _context->playerPos;
	diff *= 0.02f;
	drawBorder(v2(640, 360), diff, ds::Color(128, 60, 0, 255),0.3f);
	*/
	//_snake->render_debug();
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
	_snake->activate(1);
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void StageTest::deactivate() {
}


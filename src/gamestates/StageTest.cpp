#include "StageTest.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>
#include <utils\Profiler.h>
#include "..\GameRenderer.h"

StageTest::StageTest(GameContext* ctx) : ds::GameState("StageTest"), _context(ctx) {
	_count = 1;
	_data.count_x = 10;
	_data.count_y = 5;
	_data.border = v2(40, 40);
	_data.sides = 10;
	_data.delay = _context->settings->dodgersSpawnTimer;
	_data.type = SPT_PARTIAL_EDGES;
	_data.emitter_type = SET_IMMEDIATE;
	_stageManager = new StageManager(_context);
	_startPos = v2(1000, 710);
	_state = 1;
	_emitterTypes.push_back("IMMEDIATE");
	_emitterTypes.push_back("DELAYED");
	_enemyTypes.push_back("Dodgers");
	_enemyTypes.push_back("Bouncer");
	_enemyTypes.push_back("Snake");
	_showEditor = true;
	_enemySelection = 2;
	_emitterSelection = 0;
}

StageTest::~StageTest() {	
	delete _stageManager;
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int StageTest::update(float dt) {
	_context->world->tick(dt);
	_context->particles->update(dt);
	_context->trails->tick(dt);
	const ds::ActionEventBuffer& buffer = _context->world->getEventBuffer();
	_stageManager->tick(dt);
	_stageManager->handleEvents(buffer);
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
			gui::InputInt(10, "Enemies", &_count);
			gui::InputInt(6, "Count X", &_data.count_x);
			gui::InputInt(7, "Count Y", &_data.count_y);
			gui::InputVec2(8, "Border", &_data.border);
			gui::InputInt(9, "Sides", &_data.sides);
			gui::InputFloat(11, "Delay", &_data.delay);
			gui::ComboBox(12, _emitterTypes, &_emitterSelection, &offset, 2);
			gui::ComboBox(13, _enemyTypes, &_enemySelection, &offset, 3);
			gui::beginGroup();
			if (gui::Button(3, "Kill all")) {
				_stageManager->killAll();
			}
			if (gui::Button(3, "Start")) {
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
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void StageTest::activate() {
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void StageTest::deactivate() {
}


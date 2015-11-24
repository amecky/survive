#include "StageTest.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>
#include <utils\Profiler.h>

StageTest::StageTest(GameContext* ctx) : ds::GameState("StageTest"), _context(ctx) {
	SpawnerData data;
	data.count_x = 10;
	data.count_y = 5;
	data.border = v2(40, 40);
	//_spawner = new EdgesSpawner(data);
	_spawner = new PartialEdgesSpawner(10, data);
	_spawner->rebuild();
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int StageTest::update(float dt) {
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void StageTest::render() {
	for (int i = 0; i < _spawner->size(); ++i) {
		const v2& p = _spawner->next();
		ds::sprites::draw(p, ds::math::buildTexture(0, 0, 40, 40));
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


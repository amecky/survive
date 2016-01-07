#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include "utils\BorderLines.h"
#include "Player.h"
#include <memory\DefaultAllocator.h>

enum {ASTEROID_MAX_SEGMENTS = 36};

class TestMe : public ds::GameState {

struct Asteroid {
	v2 position;
	v2 velocity;
	float radius;
	int numSegments;
	float radiusVariance[ASTEROID_MAX_SEGMENTS];
	float rotation;
};

struct Dodger {
	v2 position;
	v2 velocity;
	v2 acceleration;
	float wobble;
	float timer;
	float angle;
};

typedef std::vector<Dodger> Dodgers;

public:
	TestMe(GameContext* ctx);
	virtual ~TestMe() {
		//delete _borderLines;
		delete _player;
	}
	void init();
	const char* getTitle() {
		return "TestMe";
	}
	int update(float dt);
	void render();
	virtual void OnChar(char ascii, unsigned int keyState);
	virtual void OnButtonUp(int button, int x, int y);
	virtual void OnButtonDown(int button, int x, int y);
	void activate();
private:
	void renderAsteroid(const Asteroid& asteroid);
	void drawCircle(const v2& pos, int segments, float radius, float thickness, const ds::Texture& t, float angleOffset = 0.0f);
	void drawSegment(const v2& pos, float alpha, float ra, float beta, float rb,float thickness,const ds::Texture& t);
	void testMem();
	void stringTest();
	ds::Texture _texture;
	Dodgers _dodgers;
	ds::Shape _shape;
	BorderLines* _borderLines;
	float _timer;
	Asteroid _asteroid;
	GameContext* _context;
	Player* _player;
	ds::DefaultAllocator alloc;
};
#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\BaseApp.h>
#include <dxstdafx.h>
#include <renderer\render_types.h>
#include "utils\BorderLines.h"

enum {ASTEROID_MAX_SEGMENTS = 36};

class TestMe : public ds::BaseApp {

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
	TestMe();
	virtual ~TestMe() {
	}
	bool loadContent();
	const char* getTitle() {
		return "TestMe";
	}
	void update(float dt);
	void draw();
	virtual void OnChar(char ascii, unsigned int keyState);
	virtual void OnButtonUp(int button, int x, int y);
	virtual void OnButtonDown(int button, int x, int y);
private:
	void renderAsteroid(const Asteroid& asteroid);
	void drawCircle(const v2& pos, int segments, float radius, float thickness, const ds::Texture& t, float angleOffset = 0.0f);
	void drawSegment(const v2& pos, float alpha, float ra, float beta, float rb,float thickness,const ds::Texture& t);
	v2 align(int index);
	v2 separate(int index);
	v2 seek(int index,const v2& target);
	ds::Texture _texture;
	Dodgers _dodgers;
	ds::Shape _shape;
	BorderLines* _borderLines;
	float _timer;
	Asteroid _asteroid;
	GameContext _context;
	GameSettings _gameSettings;
	int _states[16];
};
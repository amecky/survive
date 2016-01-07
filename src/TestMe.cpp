#include "TestMe.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>
#include "utils\FadingMessage.h"
#include "GameRenderer.h"
#include <lib\collection_types.h>
#include <utils\StringUtils.h>
#include <memory\DefaultAllocator.h>

struct Tester {
	int value;

	Tester() : value(1) {
		LOG << "created";
	}
	~Tester() {
		LOG << "destroyed (" << value << ")";
	}
};

TestMe::TestMe(GameContext* ctx) : ds::GameState("TestState") , _context(ctx) {
	_player = new Player(ctx);
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
void TestMe::init() {
	/*
	_texture = ds::math::buildTexture(515, 80, 64, 64);
	for (int i = 0; i < 5; ++i) {
		Dodger d;
		d.position = v2(100, 100 + i * 100);
		d.wobble = 0.0f;
		d.timer = 0.0f;
		d.angle = 0.0f;
		d.velocity = v2(100, 0);
		_dodgers.push_back(d);
	}
	_shape.v[0] = v2(100, 200);
	_shape.v[1] = v2(300, 200);
	_shape.v[2] = v2(300, 100);
	_shape.v[3] = v2(100, 100);
	_shape.color = ds::Color::WHITE;

	_timer = 0.0f;

	_asteroid.position = v2(512, 384);
	_asteroid.velocity = v2(100, 100);
	_asteroid.radius = 100.0f;
	for (int i = 0; i < 12; ++i) {
		_asteroid.radiusVariance[i] = ds::math::random(-10, 10);
	}
	_asteroid.numSegments = 12;
	_asteroid.rotation = 0.0f;

	_borderLines = new BorderLines(_context);
	*/

	//_player->create();
	
}

void TestMe::activate() {
	LOG << "------------------------------------------------------------";
	ds::Array<Tester> arr;
	ds::Array<Tester> ar(4);
	LOG << "at 2: " << ar[2].value;
	LOG << "size: " << ar.size();
	LOG << "capacity: " << ar.capacity();
	Tester m;
	m.value = 100;
	ar.push_back(m);
	LOG << "size after push: " << ar.size();
	LOG << "value: " << ar[4].value;
	ar.remove(2);
	LOG << "size after remove: " << ar.size();
	ds::Array<Tester>::iterator it = ar.begin();
	while (it != ar.end()) {
		LOG << it->value;
		++it;
	}
	it = ar.begin();
	while (it != ar.end()) {
		if (it->value > 10) {
			LOG << "---> removing";
			it = ar.remove(it);
		}
		else {
			++it;
		}
	}
	ar.pop_back();
	LOG << "######";
	Tester t1;
	t1.value = 200;
	ar.insert(ar.begin(), t1);
	Tester t2;
	t2.value = 300;
	ar.insert(ar.begin() + 2, t2);
	it = ar.begin();
	while (it != ar.end()) {
		LOG << it->value;
		++it;
	}
	LOG << "new capacity: " << ar.capacity();
	ar.clear();
	LOG << "new size: " << ar.size();
	ds::Array<int> nar(4);
	LOG << "------------------------------------------------------------";
	ds::Array<int> tar(8);
	for (int i = 0; i < 8; ++i) {
		tar[i] = i;
	}
	for (uint32 s = 0; s < tar.size(); ++s) {
		LOG << "tar (" << s << ") : " << tar[s];
	}
	tar.insert(tar.begin(), 100);
	tar.insert(tar.begin() + 8, 300);
	for (uint32 s = 0; s < tar.size(); ++s) {
		LOG << "tar (" << s << ") : " << tar[s];
	}
	ds::StringStream b;
	b << "Hello " << 42 << " and " << v2(200, 200) << " which will be nearly " << 123.43f << " in total ";
	b.format("%1d %1d", 200, 200);
	LOG << b.c_str();
	//messages::fadingMessage(_context, "get_ready", _context->settings->warmUpTime * 2.0f);
	//testMem();
	ds::gDefaultMemory->debug();
}

void TestMe::testMem() {
	
	void* f = alloc.allocate(16);
	void* s = alloc.allocate(16);
	void* t = alloc.allocate(12);
	void* r = alloc.allocate(6);
	alloc.deallocate(f);
	void* u = alloc.allocate(12);
	alloc.deallocate(s);
	alloc.deallocate(t);
	alloc.deallocate(r);
	alloc.deallocate(r);
	alloc.debug();
}

void TestMe::renderAsteroid(const Asteroid& asteroid) {
	float alpha = _asteroid.rotation;
	float step = TWO_PI / static_cast<float>(asteroid.numSegments);
	for (int i = 0; i < asteroid.numSegments - 1; ++i) {
		drawSegment(_asteroid.position, alpha, asteroid.radius + asteroid.radiusVariance[i], alpha + step, asteroid.radius + asteroid.radiusVariance[i + 1], 15.0f, ds::math::buildTexture(100.0f, 0.0f, 40.0f, 15.0f));
		alpha += step;
	}
	drawSegment(_asteroid.position, alpha, asteroid.radius + asteroid.radiusVariance[asteroid.numSegments-1], alpha + step, asteroid.radius + asteroid.radiusVariance[0], 15.0f, ds::math::buildTexture(100.0f, 0.0f, 40.0f, 15.0f));
}
// -------------------------------------------------------
// draw circle
// -------------------------------------------------------
void TestMe::drawCircle(const v2& pos, int segments, float radius, float thickness, const ds::Texture& t,float angleOffset) {
	float alpha = angleOffset;
	float step = TWO_PI / static_cast<float>(segments);
	for (int i = 0; i < segments; ++i) {
		drawSegment(pos, alpha, radius, alpha + step, radius, thickness, t);
		alpha += step;
	}
}

// -------------------------------------------------------
// draw segment
// -------------------------------------------------------
void TestMe::drawSegment(const v2& pos,float alpha, float ra, float beta, float rb,float thickness,const ds::Texture& t) {
	_shape.v[0] = v2(pos.x + cos(beta) * (rb + thickness), pos.y + sin(beta) * (rb + thickness));
	_shape.v[1] = v2(pos.x + cos(alpha) * (ra + thickness), pos.y + sin(alpha) * (ra + thickness));
	_shape.v[2] = v2(pos.x + cos(alpha) * ra, pos.y + sin(alpha) * ra);
	_shape.v[3] = v2(pos.x + cos(beta) * rb, pos.y + sin(beta) * rb);		
	ds::sprites::draw(_shape, t);
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
int TestMe::update(float dt) {
	_context->world->tick(dt);
	return 0;
}

void TestMe::render() {

}
// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void TestMe::OnChar(char ascii, unsigned int keyState) {
}


// -------------------------------------------------------
// OnButtonUp
// -------------------------------------------------------
void TestMe::OnButtonUp(int button, int x, int y) {

}

void TestMe::OnButtonDown(int button, int x, int y) {
}



#include "TestMe.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>

//ds::BaseApp *app = new TestMe();

TestMe::TestMe() : ds::BaseApp() {
	//_CrtSetBreakAlloc(6297);
	m_Width = 1024;
	m_Height = 768;
	m_ClearColor = ds::Color(0.0f,0.0f,0.0f,1.0f);	
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool TestMe::loadContent() {
	int texture = ds::renderer::loadTexture("TextureArray");
	assert(texture != -1);
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

	_borderLines = new BorderLines(0);

	return true;
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
// https://processing.org/examples/flocking.html
void TestMe::update(float dt) {
	_timer += dt;

	_borderLines->update(dt);
	
	v2 mp = ds::renderer::getMousePosition();
	v2 v;
	for (size_t i = 0; i < _dodgers.size(); ++i) {
		Dodger& d = _dodgers[i];
		v2 acceleration = v2(0, 0);
		acceleration += seek(i,mp);
		acceleration += align(i);
		acceleration += separate(i);
		d.velocity += acceleration * dt;
		d.position += d.velocity * dt;
		d.angle = ds::math::getTargetAngle(d.velocity, V2_RIGHT);
	}
}

v2 TestMe::seek(int index, const v2& target) {
	v2 desired = target - _dodgers[index].position;
	// Scale to maximum speed
	normalize(desired);
	desired.x *= 1.5f;
	desired.y *= 1.5f;
	// Steering = Desired minus Velocity
	v2 steer = desired - _dodgers[index].velocity;
	return steer;
}

v2 TestMe::align(int index) {
	v2 sum(0, 0);
	float dist = 40.0f;
	int count = 0;
	for (size_t i = 0; i < _dodgers.size(); ++i) {
		if (i != index) {
			Dodger& d = _dodgers[i];
			v2 diff = d.position - _dodgers[index].position;
			if (sqr_length(diff) < (dist * dist)) {
				++count;
				sum += d.velocity;
			}
		}
	}
	if (count > 0) {
		sum.x /= count;
		sum.y /= count;
		// Implement Reynolds: Steering = Desired - Velocity
		normalize(sum);
		sum.x *= 1.2f;
		sum.y *= 1.2f;
		//sum.mult(maxspeed);
		v2 steer = sum - _dodgers[index].velocity;
		//clamp(&sum, v2(0, 0), v2(100, 100));// steer.limit(maxforce);
		return steer;
	}
	else {
		return v2(0, 0);
	}
}

v2 TestMe::separate(int index) {
	float desiredseparation = 25.0f;
	v2 steer(0, 0);
	float dist = 40.0f;
	int count = 0;
	// For every boid in the system, check if it's too close
	for (size_t i = 0; i < _dodgers.size(); ++i) {
		if (i != index) {
			Dodger& d = _dodgers[i];
			v2 diff = _dodgers[index].position - d.position;
			if (sqr_length(diff) < (dist * dist)) {
				float d = length(diff);
				normalize(diff);
				diff.x /= d;
				diff.y /= d;
				steer += diff;
				++count;
			}
		}
	}
	// Average -- divide by how many
	if (count > 0) {
		steer.x /= count;
		steer.y /= count;
	}

	// As long as the vector is greater than 0
	if (sqr_length(steer) > 0.0f ) {
		// Implement Reynolds: Steering = Desired - Velocity
		normalize(steer);
		steer.x *= 200.0f;
		steer.y *= 200.0f;
		steer -= _dodgers[index].velocity;
	}
	return steer;
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void TestMe::draw() {
	_borderLines->draw();
	//drawCircle(v2(512, 384), 12, 100.0f, 15.0f,ds::math::buildTexture(100.0f, 0.0f, 40.0f, 15.0f),m_GameTime.totalTime);
	//for (size_t i = 0; i < _dodgers.size(); ++i) {
		//ds::sprites::draw(_dodgers[i].position, _texture, _dodgers[i].angle);
	//}
	renderAsteroid(_asteroid);
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



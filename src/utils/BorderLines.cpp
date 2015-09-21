#include "BorderLines.h"
#include <sprites\SpriteBatch.h>

// -------------------------------------------------------------
// BorderLines
// -------------------------------------------------------------
BorderLines::BorderLines(GameContext* context) : _context(context) {
	_shape.color = ds::Color::WHITE;
	for (int i = 0; i < 18; ++i) {
		_lines[0].points[i] = v2(25, 40 + i * 40);
		float a = TWO_PI * static_cast<float>(i) / 4.0f + ds::math::random(0.0f, PI*0.25f);
		_lines[0].angle[i] = a;
	}
	_lines[0].num = 18;
	_lines[0].texture = ds::math::buildTexture(0.0f, 50.0f, 11.0f, 40.0f);
	_lines[0].orientation = LO_HORIZONTAL;

	for (int i = 0; i < 18; ++i) {
		_lines[1].points[i] = v2(980, 40 + i * 40);
		_lines[1].angle[i] = TWO_PI * static_cast<float>(i) / 4.0f + ds::math::random(0.0f, PI*0.25f);
	}
	_lines[1].num = 18;
	_lines[1].texture = ds::math::buildTexture(0.0f, 50.0f, 11.0f, 40.0f);
	_lines[1].orientation = LO_HORIZONTAL;
	for (int i = 0; i < 25; ++i) {
		_lines[2].points[i] = v2(25 + i * 40, 40);
		_lines[2].angle[i] = TWO_PI * static_cast<float>(i) / 4.0f + ds::math::random(0.0f, PI*0.25f);
	}
	_lines[2].num = 25;
	_lines[2].texture = ds::math::buildTexture(0.0f, 62.0f, 40.0f, 11.0f);
	_lines[2].orientation = LO_VERTICAL;
	for (int i = 0; i < 25; ++i) {
		_lines[3].points[i] = v2(25 + i * 40, 720);
		_lines[3].angle[i] = TWO_PI * static_cast<float>(i) / 4.0f + ds::math::random(0.0f, PI*0.25f);
	}
	_lines[3].num = 25;
	_lines[3].texture = ds::math::buildTexture(0.0f, 62.0f, 40.0f, 11.0f);
	_lines[3].orientation = LO_VERTICAL;
}

// -------------------------------------------------------------
// update
// -------------------------------------------------------------
void BorderLines::update(float dt) {
	for (int j = 0; j < 4; ++j) {
		for (int i = 1; i < _lines[j].num - 1; ++i) {
			_lines[j].angle[i] += dt * _context->settings->borderAmplitude;
		}
	}
}

// -------------------------------------------------------------
// render line
// -------------------------------------------------------------
void BorderLines::renderLine(const Line& l) {
	_shape.color = ds::Color::WHITE;
	v2 prev = l.points[0];
	float dx = 0.0f;
	if (l.orientation == LO_HORIZONTAL) {
		dx = l.texture.dim.x;
	}
	else {
		dx = l.texture.dim.y;
	}
	float amplitude = _context->settings->borderShakeRadius;
	prev.x += sin(l.angle[0]) * amplitude;
	for (int i = 1; i < l.num; ++i) {
		v2 p = l.points[i];
		if (l.orientation == LO_HORIZONTAL) {
			p.x += sin(l.angle[i]) * amplitude;		
			_shape.v[0] = v2(p.x, p.y);
			_shape.v[1] = v2(p.x + dx, p.y);
			_shape.v[2] = v2(prev.x + dx, prev.y);
			_shape.v[3] = v2(prev.x, prev.y);
		}
		else {
			p.y += sin(l.angle[i]) * amplitude;
			_shape.v[0] = v2(prev.x, prev.y + dx);
			_shape.v[1] = v2(p.x, p.y + dx);
			_shape.v[2] = v2(p.x, p.y);
			_shape.v[3] = v2(prev.x, prev.y);
		}
		ds::sprites::draw(_shape, l.texture);
		prev = l.points[i];
		if (l.orientation == LO_HORIZONTAL) {
			prev.x += sin(l.angle[i]) * amplitude;
		}
		else{
			prev.y += sin(l.angle[i]) * amplitude;
		}
	}
}

// -------------------------------------------------------------
// draw
// -------------------------------------------------------------
void BorderLines::draw() {
	for (int j = 0; j < 4; ++j) {
		renderLine(_lines[j]);
	}
}
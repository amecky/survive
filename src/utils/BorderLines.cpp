#include "BorderLines.h"
#include <sprites\SpriteBatch.h>
#include "..\Constants.h"

Border::Border(GameContext* context) : _context(context), _world(context->world) {
	_border_color = ds::Color(64, 64, 64, 255);
	create();
}

void Border::hitBorder(const v2& pos) {
	int x = (pos.x - 30) / 40;
	int y = (pos.y - 30) / 40;
	int index = x + y * 30;
	for (int i = 0; i < _borderTiles.size(); ++i) {
		v2 d = _borderTiles[i].position - pos;
		if (sqr_length(d) < 40.0f * 40.0f) {
			if (_borderTiles[i].type == OT_TOP_WALL) {
				_world->startBehavior(_borderTiles[i].sid, "top_wall_impact");
			}
			else {
				_world->startBehavior(_borderTiles[i].sid, "wall_impact");
			}
		}
	}
}

void Border::create() {
	for (int i = 0; i < 15; ++i) {
		BorderTile tl;
		v2 p = v2(30, 60 + i * 40);
		tl.sid = _world->create(p, ds::math::buildTexture(440, 20, 6, 40), 0.0f, 1.0f, 1.0f, _border_color, OT_LEFT_WALL, BORDER_LAYER);
		tl.position = p;
		tl.type = OT_LEFT_WALL;
		_borderTiles.push_back(tl);
		BorderTile tr;
		p = v2(1250, 60 + i * 40);
		tr.sid = _world->create(p, ds::math::buildTexture(440, 80, 6, 40), 0.0f, 1.0f, 1.0f, _border_color, OT_LEFT_WALL, BORDER_LAYER);
		tr.position = p;
		tr.type = OT_LEFT_WALL;
		_borderTiles.push_back(tr);
	}
	// bottom and top wall
	for (int i = 0; i < 30; ++i) {
		BorderTile tt;
		v2 p = v2(60 + i * 40, 650);
		tt.sid = _world->create(p, ds::math::buildTexture(440, 40, 40, 6), 0.0f, 1.0f, 1.0f, _border_color, OT_TOP_WALL, BORDER_LAYER);
		tt.position = p;
		tt.type = OT_TOP_WALL;
		_borderTiles.push_back(tt);
		BorderTile tb;
		p = v2(60 + i * 40, 30);
		tb.sid = _world->create(p, ds::math::buildTexture(440, 40, 40, 6), 0.0f, 1.0f, 1.0f, _border_color, OT_TOP_WALL, BORDER_LAYER);
		tb.position = p;
		tb.type = OT_TOP_WALL;
		_borderTiles.push_back(tb);
	}
}




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
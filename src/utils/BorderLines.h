#pragma once
#include "base\BaseApp.h"
#include "dxstdafx.h"
#include <renderer\render_types.h>
#include "GameContext.h"

class BorderLines  {

enum LineOrientation {
	LO_VERTICAL,
	LO_HORIZONTAL
};

struct Line {
	v2 points[32];
	float angle[32];
	int num;
	ds::Texture texture;
	LineOrientation orientation;
};

public:
	BorderLines(GameContext* context);
	~BorderLines() {}
	void update(float dt);
	void draw();
private:
	void renderLine(const Line& l);
	ds::Shape _shape;
	Line _lines[4];
	float _timer;
	GameContext* _context;
};


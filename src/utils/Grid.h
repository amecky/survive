#pragma once
#include <lib\HexGrid.h>
#include <utils\Color.h>

const ds::Color BACKGROUND_COLOR = ds::Color(32, 32, 32, 64);

struct GridItem {

	int type;
	float timer;
	bool highlighted;
	ds::Color color;

	GridItem() : type(1), timer(0.0f), highlighted(false) , color(BACKGROUND_COLOR) {}

};

class Grid : public ds::HexGrid<GridItem> {

public:
	Grid();
	virtual ~Grid() {}
	void tick(float dt);
	void highlight(const v2& position, const ds::Color& color);
	void render();

};


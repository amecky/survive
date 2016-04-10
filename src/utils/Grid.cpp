#include "Grid.h"
#include <sprites\SpriteBatch.h>
#include <math\GameMath.h>

Grid::Grid() : ds::HexGrid<GridItem> () {
	GridItem item;
	resize(30, 17, item);
	setOrigin(v2(30, 20));
}


void Grid::tick(float dt) {
	for (int i = 0; i < size(); ++i) {
		GridItem& item = get(i);
		if (item.highlighted) {
			item.timer -= dt;
			if (item.timer <= 0.0f) {
				item.color = BACKGROUND_COLOR;
				item.highlighted = false;
			}
		}
	}
}

void Grid::highlight(const v2& position, const ds::Color& color) {
	Hex h = convert(position);
	if (isValid(h)) {
		int current = getIndex(h);
		//if (current != _hover) {
		//_hover = current;
		GridItem& here = get(current);
		here.highlighted = true;
		here.timer = 0.5f;
		here.color = color;
		Hex neighbours[6];
		int num = neighbors(h, neighbours);
		for (int i = 0; i < num; ++i) {
			GridItem& n = get(neighbours[i]);
			n.highlighted = true;
			n.timer = 0.5f;
			n.color = color;
		}
	}
}

void Grid::render() {
	for (int i = 0; i < size(); ++i) {
		const GridItem& item = get(i);
		ds::Color clr = item.color;
		if (item.highlighted) {
			clr = ds::color::lerp(BACKGROUND_COLOR, item.color, item.timer / 0.5f);
			//float alpha = 64.0f + (255.0f - 64.0f) * (item.timer / 0.5f);
			//ds::sprites::draw(position(i), ds::math::buildTexture(ds::Rect(80, 480, 40, 44)), 0.0f, 1.0f, 1.0f, ds::Color(32, 32, 32, (int)alpha));
		}
		//else {
			ds::sprites::draw(position(i), ds::math::buildTexture(ds::Rect(80, 480, 40, 44)), 0.0f, 1.0f, 1.0f, clr);
		//}
	}
}
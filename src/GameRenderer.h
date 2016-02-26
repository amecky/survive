#pragma once
#include "utils\GameContext.h"

class GameRenderer {

	struct BorderTile {
		ds::SID sid;
		v2 position;
	};

public:
	GameRenderer(GameContext* context);
	~GameRenderer();
	void renderWorld();
	void hitBorder(const v2& pos);
private:
	void drawBorder();
	void createBorder();
	ds::Color _border_color;
	GameContext* _context;
	int m_AddBS;
	ds::RTID _rt1;
	int _shakeShader;
	ds::RTID _rt2;
	bool m_Shaking;
	float m_ShakeTimer;
	int _light_desc;
	ds::Shader* _shader;
	ds::Array<BorderTile> _borderTiles;
};


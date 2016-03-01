#pragma once
#include "utils\GameContext.h"

class GameRenderer {

	

public:
	GameRenderer(GameContext* context);
	~GameRenderer();
	void renderWorld();
private:
	GameContext* _context;
	int m_AddBS;
	ds::RTID _rt1;
	int _shakeShader;
	ds::RTID _rt2;
	bool m_Shaking;
	float m_ShakeTimer;
	int _light_desc;
	ds::Shader* _shader;
	
};


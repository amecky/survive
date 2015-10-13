#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include "utils\BorderLines.h"
#include <ui\IMGUI.h>

class GUITest : public ds::GameState {

public:
	GUITest();
	virtual ~GUITest() {}	
	void init();
	int update(float dt);
	void render();
private:
	float _test;
	int _iTest;
	v2 _v2Test;
	v3 _v3Test;
	ds::Rect _rectTest;
	int _state;
	int _state2;
	std::vector<std::string> _items;
	int _selected;
	int _dpState;
	ds::Color _clr;
	bool _checkBox;
	char _name[32];
	v2 _startPos;
	int _comboOffset;
};
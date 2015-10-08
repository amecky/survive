#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\BaseApp.h>
#include <dxstdafx.h>
#include <renderer\render_types.h>
#include "utils\BorderLines.h"
#include <ui\IMGUI.h>

class GUITest : public ds::BaseApp {

public:
	GUITest();
	virtual ~GUITest() {
	}
	bool loadContent();
	const char* getTitle() {
		return "GUITest";
	}
	void update(float dt);
	virtual void draw();
	virtual void OnChar(char ascii, unsigned int keyState);
	virtual void OnButtonUp(int button, int x, int y);
	virtual void OnButtonDown(int button, int x, int y);
	virtual void OnKeyUp(WPARAM wParam);
private:
	float _test;
	int _iTest;
	v2 _v2Test;
	v3 _v3Test;
	ds::Rect _rectTest;
	int _state;
	std::vector<std::string> _items;
	int _selected;
	int _dpState;
	ds::Color _clr;
	bool _checkBox;
};
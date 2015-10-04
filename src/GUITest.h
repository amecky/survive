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
#include "gui\GUI.h"

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
	void draw();
	virtual void OnChar(char ascii, unsigned int keyState);
	virtual void OnButtonUp(int button, int x, int y);
	virtual void OnButtonDown(int button, int x, int y);
private:
	GUI _gui;
};
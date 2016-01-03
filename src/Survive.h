#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include "dxstdafx.h"
#include "utils\SettingsConverter.h"
#include "utils\GameContext.h"

class Survive : public ds::BaseApp {

public:	
	Survive();
	virtual ~Survive();
	bool loadContent();
	void init();
	const char* getTitle() {
		return "Survive";
	}
	void update(float dt);
	void draw();
	void OnChar(char ascii, unsigned int keyState);
private:
	void onGUIButton( ds::DialogID dlgID,int button );
	GameContext* _context;
	bool _showSettings;
	v2 _settingPos;
};
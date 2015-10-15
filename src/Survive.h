#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include "dxstdafx.h"
#include <renderer\render_types.h>
#include <objects\HUD.h>
#include "Worms.h"
#include "utils\SettingsConverter.h"

class Survive : public ds::BaseApp {

enum GameMode {
	GM_START,
	GM_RUNNING,
	GM_OVER
};

public:	
	Survive();
	virtual ~Survive() {
		delete _gameSettings;
	}	
	bool loadContent();
	void init();
	const char* getTitle() {
		return "Survive";
	}
	void update(float dt);
	void draw();
	virtual void OnChar( char ascii,unsigned int keyState );
	virtual void OnButtonUp( int button,int x,int y );
	virtual void OnButtonDown( int button,int x,int y );
private:
	void stopGame();
	void restart();
	void onGUIButton( ds::DialogID dlgID,int button );

	float m_Timer;
	GameMode m_Mode;
	GameSettings* _gameSettings;
	bool _showGameStates;
	bool _showSettings;
	v2 _settingsPosition;
	int _settingsStates[16];
	
};
#include "Survive.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <base\GameStateMachine.h>
#include "GUITest.h"
#include "TestMe.h"

ds::BaseApp *app = new Survive(); 

Survive::Survive() : ds::BaseApp() {
	//_CrtSetBreakAlloc(79);
	_settings.screenWidth = 1024;
	_settings.screenHeight = 768;
	_settings.clearColor = ds::Color(0.0f,0.0f,0.0f,1.0f);	
	m_Timer = 0.0f;
	m_Mode = GM_START;
	_gameSettings = new GameSettings;
	_showSettings = true;
	for (int i = 0; i < 16; ++i) {
		_settingsStates[i] = 1;
	}
	_settingsPosition = v2(800, 740);
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Survive::loadContent() {	
	int texture = ds::renderer::loadTexture("TextureArray");
	assert( texture != -1 );	
	
	ds::BitmapFont* font = ds::renderer::createBitmapFont("xscale");
	ds::assets::load("xscale", font, ds::CVT_FONT);
	ds::renderer::initializeBitmapFont(font,texture);	
	//initializeGUI();
	gui::initialize();
	settings::load(_gameSettings);
	//m_Game = new Worms();
	//m_Game->init(m_Settings,font,&gui);
	stateMachine->add(new GUITest());
	stateMachine->add(new TestMe());
	//gui.activate("GameOver");
	_showGameStates = false;
	return true;
}

void Survive::init() {
	stateMachine->activate("GUITest");
}

// -------------------------------------------------------
// Restart game
// -------------------------------------------------------
void Survive::restart() {
}
// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Survive::update(float dt) {
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Survive::draw() {	
	if (_showGameStates) {
		stateMachine->showDialog();
	}
	if (_showSettings) {
		settings::showDialog(_gameSettings, &_settingsPosition, _settingsStates);
	}
}

// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void Survive::OnChar( char ascii,unsigned int keyState ) {
	if (ascii == 'd') {
		_showGameStates = !_showGameStates;
	}
	if (ascii == 's') {
		_showSettings = !_showSettings;
	}
	
}

// -------------------------------------------------------
// Stop game
// -------------------------------------------------------
void Survive::stopGame() {
}

// -------------------------------------------------------
// OnButtonUp
// -------------------------------------------------------
void Survive::OnButtonUp( int button,int x,int y ) {
}

void Survive::onGUIButton( ds::DialogID dlgID,int button ) {
}

void Survive::OnButtonDown( int button,int x,int y ) {
}

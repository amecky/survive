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
	_settings.showEditor = true;
	m_Timer = 0.0f;
	_context = new GameContext;
	_context->settings = new GameSettings;
	_context->world = new ds::World;
	_context->trails = new Trail(_context);
	_context->particles = particles;
	_context->playerSpeed = 200.0f;
	_context->doubleFire = false;
	_context->fireRate = 0.4f;
	_context->tripleShot = false;
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
	ds::BitmapFont* font = ds::assets::loadFont("xscale", texture);
	ds::sprites::initializeTextSystem(font);
	gui::initialize();
	initializeGUI(font);
	_context->settings->load();
	stateMachine->add(new GUITest());
	stateMachine->add(new TestMe(_context));
	stateMachine->add(new Worms(_context));
	_showGameStates = false;
	return true;
}

void Survive::init() {
	stateMachine->activate("MainGameState");
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
		_context->settings->showDialog();
	}
}

// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void Survive::OnChar( char ascii,unsigned int keyState ) {
	if (ascii == '1') {
		_showGameStates = !_showGameStates;
	}
	if (ascii == '2') {
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

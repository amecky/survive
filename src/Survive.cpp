#include "Survive.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <base\GameStateMachine.h>
#include "GUITest.h"
#include "TestMe.h"
#include "Worms.h"
#include "gamestates\GameOverState.h"
#include "gamestates\StageTest.h"
#include "GameRenderer.h"

ds::BaseApp *app = new Survive(); 

Survive::Survive() : ds::BaseApp() {
	//_CrtSetBreakAlloc(79);
	// 720p
	_settings.screenWidth = 1280;
	_settings.screenHeight = 720;
	_settings.clearColor = ds::Color(0.0f,0.0f,0.0f,1.0f);	
	_settings.initializeEditor = true;
	_context = new GameContext;
	_context->settings = new GameSettings;
	_context->world = new ds::World;
	_context->trails = new Trail(_context,512);
	_context->particles = particles;
	_context->playerSpeed = 300.0f;
	_context->doubleFire = false;
	_context->fireRate = 0.4f;
	_context->tripleShot = false;
	_context->world_pos = v2(960, 540);
	_showSettings = false;
	activateMonitoring(12.0f);
	_settingPos = v2(900, 710);
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
	_context->hud = gui.get("HUD");
	_context->settings->load();
	_context->renderer = new GameRenderer(_context);
	ds::assets::loadSpriteTemplates();
	ds::assets::loadParticleSystem("particlesystems", _context->particles);
	stateMachine->add(new GUITest());
	stateMachine->add(new TestMe(_context));
	stateMachine->add(new Worms(_context));
	stateMachine->add(new StageTest(_context));
	stateMachine->add(new GameOverState(_context,&gui));
	stateMachine->add(new ds::BasicMenuGameState("MainMenuState", "MainMenu", &gui));
	stateMachine->connect("MainGameState", 1, "GameOverState");
	stateMachine->connect("GameOverState", 1, "MainGameState");
	stateMachine->connect("MainMenuState", 2, "TestState");
	stateMachine->connect("MainMenuState", 1, "MainGameState");
	return true;
}

void Survive::init() {
	stateMachine->activate("MainMenuState");
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
	if (_showSettings) {
		_context->settings->showDialog(&_settingPos);
	}
}

// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void Survive::OnChar( char ascii,unsigned int keyState ) {
	if (ascii == '2') {
		_showSettings = !_showSettings;
	}
	
}

// -------------------------------------------------------
// OnButtonUp
// -------------------------------------------------------
void Survive::onGUIButton( ds::DialogID dlgID,int button ) {
}


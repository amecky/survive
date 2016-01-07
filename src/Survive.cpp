#include "Survive.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <base\GameStateMachine.h>
#include "TestMe.h"
#include "Worms.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainGameState.h"
#include "GameRenderer.h"
#include <io\FileRepository.h>

ds::BaseApp *app = new Survive(); 

Survive::Survive() : ds::BaseApp() {
	//_CrtSetBreakAlloc(7391);
}

Survive::~Survive() {
	delete _context->settings;
	delete _context->trails;
	delete _context->world;
	delete _context->renderer;
	delete _context;
}
// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Survive::loadContent() {	
	_context = new GameContext;
	_context->settings = new GameSettings;
	_context->world = new ds::World;
	_context->trails = new Trail(_context, 512);
	_context->particles = particles;
	_context->playerSpeed = 300.0f;
	_context->doubleFire = false;
	_context->fireRate = 0.4f;
	_context->tripleShot = false;
	_context->world_pos = v2(960, 540);
	_showSettings = false;
	activateMonitoring(12.0f);
	_settingPos = v2(900, 710);
	_context->hud = gui->get("HUD");
	_context->settings->load();
	_context->renderer = new GameRenderer(_context);
	addGameState(new TestMe(_context));
	addGameState(new Worms(_context));
	addGameState(new MainGameState(_context));
	addGameState(new GameOverState(_context, gui));
	addGameState(new ds::BasicMenuGameState("MainMenuState", "MainMenu", gui));
	connectGameStates("MainGameState", 1, "GameOverState");
	connectGameStates("GameOverState", 1, "MainGameState");
	connectGameStates("MainMenuState", 2, "TestState");
	connectGameStates("MainMenuState", 1, "MainGameState");
	return true;
}

void Survive::init() {
	activate("TestState");
	//ds::repository::list();
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
	LOG << "dialog: " << dlgID << " button: " << button;
}


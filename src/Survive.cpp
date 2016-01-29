#include "Survive.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <base\GameStateMachine.h>
#include "gamestates\GameOverState.h"
#include "gamestates\MainGameState.h"
#include "GameRenderer.h"
#include <io\FileRepository.h>
#include <renderer\Scenes.h>

ds::BaseApp *app = new Survive(); 

Survive::Survive() : ds::BaseApp() {
	//_CrtSetBreakAlloc(7391);
}

Survive::~Survive() {
	delete _context->settings;
	delete _context->trails;
	delete _context->renderer;
	delete _context;
}
// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Survive::loadContent() {	
	_context = new GameContext;
	_context->settings = new GameSettings;
	_context->settings->load();
	_context->world = world;
	_context->trails = new Trail(_context, 512);
	_context->particles = particles;
	_context->playerSpeed = 300.0f;
	_context->doubleFire = false;
	_context->fireRate = 0.4f;
	_context->tripleShot = false;
	_context->world_pos = v2(960, 540);
	_showSettings = false;
	//activateMonitoring(12.0f);
	_settingPos = v2(900, 710);
	_context->hud = gui->get("HUD");
	_context->settings->load();
	_context->renderer = new GameRenderer(_context);
	addGameState(new MainGameState(_context));
	addGameState(new GameOverState(_context, gui));
	addGameState(new ds::BasicMenuGameState("MainMenuState", "MainMenu", gui));
	connectGameStates("MainGameState", 1, "GameOverState");
	connectGameStates("GameOverState", 1, "MainGameState");
	connectGameStates("MainMenuState", 1, "MainGameState");

	//ds::Scenes s;
	//s.load();
	return true;
}

void Survive::init() {
	activate("MainGameState");
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
}

// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void Survive::OnChar( char ascii,unsigned int keyState ) {
}

// -------------------------------------------------------
// OnButtonUp
// -------------------------------------------------------
void Survive::onGUIButton( ds::DialogID dlgID,int button ) {
	LOG << "dialog: " << dlgID << " button: " << button;
}


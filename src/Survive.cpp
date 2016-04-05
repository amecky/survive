#include "Survive.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <base\GameStateMachine.h>
#include "gamestates\GameOverState.h"
#include "gamestates\MainGameState.h"
#include <io\FileRepository.h>
#include <renderer\Scenes.h>
#include "Constants.h"

ds::BaseApp *app = new Survive(); 

Survive::Survive() : ds::BaseApp() {
	//_CrtSetBreakAlloc(7391);
}

Survive::~Survive() {
	delete _context->settings;
	delete _context->trails;
	delete _context;
}
// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Survive::loadContent() {	
	world->enableCollisionChecks();
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
	_context->world->setBoundingRect(ds::Rect(0, 0, 1600, 1200));
	_showSettings = false;
	//activateMonitoring(12.0f);
	_settingPos = v2(900, 710);
	_context->hud = gui->get("HUD");
	_context->settings->load();
	addGameState(new MainGameState(_context));
	addGameState(new GameOverState(_context, gui));
	addGameState(new ds::BasicMenuGameState("MainMenuState", "MainMenu", gui));
	connectGameStates("MainGameState", 1, "GameOverState");
	connectGameStates("GameOverState", 1, "MainGameState");
	connectGameStates("MainMenuState", 1, "MainGameState");
	/*
	if (ascii == 'x') {
	_cubes->killAll();
	}
	if (ascii == 'g') {
	_effect->activate();
	}
	if (ascii == '1') {
	//_cubes->emitt(0);
	_deathBalls->start();
	}
	if (ascii == '4') {
	_cubes->emitt(3);
	}
	if (ascii == '5') {
	_cubes->emitt(4);
	}
	if (ascii == '6') {
	_context->particles->startGroup(1, v2(800, 450));
	}
	if (ascii == '7') {
	_worm->start(v2(512, 384));
	}
	if (ascii == '8') {
	_context->particles->start(14, v2(800, 450));
	}
	if (ascii == '9') {
	v2 pp = _world->getPosition(_context->playerID);
	v2 p = util::pickSpawnPoint(pp);
	//_spawner->start(p);
	}
	if (ascii == 'r') {
	_cubes->reload();
	}
	*/
	addShortcut("End Game", 'e', GE_END_GAME);
	addShortcut("Start DB", '1', GE_START_DEATHBALL);
	addShortcut("Straight Worm", '2', GE_START_STRAIGHT_WORM);
	addShortcut("Bezier Worm", '3', GE_START_BEZIER_WORM);
	addShortcut("Ring Worm", '4', GE_START_RING_WORM);
	addShortcut("DF", '5', GE_TOGGLE_DOUBLE_FIRE);
	addShortcut("TF", '6', GE_TOGGLE_TRIPLE_FIRE);
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
// OnButtonUp
// -------------------------------------------------------
void Survive::onGUIButton( ds::DialogID dlgID,int button ) {
	LOG << "dialog: " << dlgID << " button: " << button;
}


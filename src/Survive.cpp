#include "Survive.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>


//ds::BaseApp *app = new Survive(); 

Survive::Survive() : ds::BaseApp() {
	//_CrtSetBreakAlloc(6297);
	_settings.screenWidth = 1024;
	_settings.screenHeight = 768;
	_settings.clearColor = ds::Color(0.0f,0.0f,0.0f,1.0f);	
	m_Timer = 0.0f;
	m_Mode = GM_START;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Survive::loadContent() {	
	int texture = ds::renderer::loadTexture("TextureArray");
	assert( texture != -1 );
	ds::BitmapFont* font = ds::renderer::createBitmapFont("xscale");
	ds::assets::load("xscale", font, ds::CVT_FONT);
	ds::renderer::initializeBitmapFont(*font,texture);
	initializeGUI();
	settings::load(m_Settings);
	m_Game = new Worms();
	m_Game->init(m_Settings,font,&gui);

	//gui.activate("GameOver");
	return true;
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
	m_Game->tick(dt);
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Survive::draw() {	
	m_Game->render();
}

// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void Survive::OnChar( char ascii,unsigned int keyState ) {
	m_Game->OnChar(ascii, keyState);
	if ( ascii == 'r' ) {
		restart();
	}
	if ( ascii == 'b' ) {
		m_Game->debug();
		m_Game->toggleDebugFlag();
	}	
}

// -------------------------------------------------------
// Stop game
// -------------------------------------------------------
void Survive::stopGame() {
	m_Mode = GM_OVER;
	gui.activate("GameOver");
}

// -------------------------------------------------------
// OnButtonUp
// -------------------------------------------------------
void Survive::OnButtonUp( int button,int x,int y ) {
	m_Game->stopShooting();
}

void Survive::onGUIButton( ds::DialogID dlgID,int button ) {
	LOG << "dialog " << dlgID << " button " << button;
	if ( dlgID == 0 && button == 0 ) {
		gui.deactivate("GameOver");
		m_Game->restart();
	}
	else if ( dlgID == 0 && button == 1 ) {
		gui.deactivate("GameOver");
		gui.activate("MainMenu");
	}
	else if ( dlgID == 1 && button == 0 ) {
		gui.deactivate("MainMenu");
		restart();
	}
	else if ( dlgID == 1 && button == 3 ) {
		gui.activate("Credits");
		gui.deactivate("MainMenu");
	}
	else if ( dlgID == 1 && button == 2 ) {
		gui.activate("Highscores");
		gui.deactivate("MainMenu");
	}
	else if ( dlgID == 1 && button == 1 ) {
		shutdown();
	}
	else if ( dlgID == 2 && button == 1 ) {
		gui.deactivate("Credits");
		gui.activate("MainMenu");
	}
	else if ( dlgID == 3 && button == 1 ) {
		gui.deactivate("Highscores");
		gui.activate("MainMenu");
	}
}

void Survive::OnButtonDown( int button,int x,int y ) {
	m_Game->startShooting();
}

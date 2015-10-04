#include "GUITest.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>

ds::BaseApp *app = new GUITest();

GUITest::GUITest() : ds::BaseApp() {
	//_CrtSetBreakAlloc(6297);
	_settings.screenWidth = 1024;
	_settings.screenHeight = 768;
	//m_ClearColor = ds::Color(0.0f,0.0f,0.0f,1.0f);	
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool GUITest::loadContent() {
	int texture = ds::renderer::loadTexture("TextureArray");
	assert(texture != -1);

	_gui.intialize();

	PanelWidget* panel = _gui.addPanel(v2(512, 384), v2(200, 200));
	_gui.addLabel(v2(10, 10), "Label1: 100,200",panel);
	return true;
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void GUITest::update(float dt) {
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void GUITest::draw() {
	_gui.render();
}

// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void GUITest::OnChar(char ascii, unsigned int keyState) {
}


// -------------------------------------------------------
// OnButtonUp
// -------------------------------------------------------
void GUITest::OnButtonUp(int button, int x, int y) {

}

void GUITest::OnButtonDown(int button, int x, int y) {
}



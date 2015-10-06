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

	gui::initialize();
	_test = 18.3f;
	_iTest = 1234;
	_v2Test = v2(100, 200);
	_v3Test = v3(123, 456, 789);
	_rectTest = ds::Rect(10, 20, 30, 40);
	_state = 1;
	_items.push_back("Entry 1");
	_items.push_back("Entry 2");
	_items.push_back("Entry 3");
	_items.push_back("Entry 4");
	_items.push_back("Entry 5");
	_selected = 1;
	_dpState = 1;
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
	if (gui::begin("Test", &_state, v2(100, 600))) {
		gui::Label("Hello World: 100,200");
		gui::InputFloat(8, "Float", &_test);
		gui::InputInt(3, "IntValue", &_iTest);
		gui::InputVec2(4, "Vector", &_v2Test);
		gui::InputVec3(5, "V3", &_v3Test);
		gui::InputRect(6, "Rect", &_rectTest);
		gui::ComboBox(7, _items, &_selected);
		gui::beginGroup();
		if (gui::Button(1, "OK")) {
			LOG << "Int Test: " << _iTest;
			LOG << "OK pressed";
		}
		if (gui::Button(2, "Cancel")) {
			LOG << "Cancel clicked";
			LOG << "v2Test: " << DBG_V2(_v2Test);
		}
		gui::endGroup();		
	}
	gui::end();
}

// -------------------------------------------------------
// OnChar
// -------------------------------------------------------
void GUITest::OnChar(char ascii, unsigned int keyState) {
	gui::sendKey(ascii);
}


// -------------------------------------------------------
// OnButtonUp
// -------------------------------------------------------
void GUITest::OnButtonUp(int button, int x, int y) {

}

void GUITest::OnButtonDown(int button, int x, int y) {
}

void GUITest::OnKeyUp(WPARAM wParam) {
	if (wParam == VK_BACK) {
		gui::sendKey(128);
	}
	if (wParam == VK_LEFT) {
		gui::sendKey(129);
	}
	if (wParam == VK_RIGHT) {
		gui::sendKey(130);
	}
	if (wParam == VK_HOME) {
		gui::sendKey(131);
	}
	if (wParam == VK_END) {
		gui::sendKey(132);
	}
	if (wParam == VK_RETURN) {
		gui::sendKey(133);
	}
}



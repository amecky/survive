#include "GUITest.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include <renderer\BitmapFont.h>
#include <math\GameMath.h>
#include <utils\Profiler.h>

GUITest::GUITest() : ds::GameState("GUITest") {}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
void GUITest::init() {
	
	//int texture = ds::renderer::loadTexture("TextureArray");
	//assert(texture != -1);
	_test = 18.3f;
	_iTest = 1234;
	_v2Test = v2(100, 200);
	_v3Test = v3(123, 456, 789);
	_rectTest = ds::Rect(10, 20, 30, 40);
	_state = 1;
	_state2 = 1;
	_items.push_back("Entry 1");
	_items.push_back("Entry 2");
	_items.push_back("Entry 3");
	_items.push_back("Entry 4");
	_items.push_back("Entry 5");
	_items.push_back("Entry 6");
	_items.push_back("Entry 7");
	_items.push_back("Entry 8");
	_selected = 1;
	_dpState = 0;
	_checkBox = true;
	sprintf(_name, "Hello world");
	_startPos = v2(100, 700);
	_comboOffset = 0;
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
int GUITest::update(float dt) {
	return 0;
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void GUITest::render() {
	/*
	PR_START("GUITest::render")
	gui::start(1,&_startPos);
	if (gui::begin("Test", &_state)) {
		gui::Label(12,"Hello World: 100,200");
		gui::InputFloat(8, "Float", &_test,0.0f,20.0f,0.1f);
		gui::InputInt(3, "IntValue", &_iTest);
		gui::InputVec2(4, "Vector", &_v2Test);
		gui::InputVec3(5, "V3", &_v3Test);
		gui::InputRect(6, "Rect", &_rectTest);
		gui::ComboBox(8, _items, &_selected, &_comboOffset);
		gui::DropDownBox(7, _items, &_selected,&_dpState);
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
	//gui::debugWindow();
	gui::end();

	if (gui::begin("Test2", &_state2)) {
		gui::InputColor(12, "Color", &_clr);
		gui::CheckBox(13, "Checkbox", &_checkBox);
		gui::InputInt(14, "IntStepValue", &_iTest, 0, 1000, 10);
		gui::Input(17, "String", _name, 32);
		float val[16];// = { 0.2f, 0.4f, 0.3f, 0.1f, 0.15f, 0.6f, 1.0f, 0.1f, 0.9f, 0.75f };
		int count = profiler::get_total_times(val, 16);
		gui::Histogram(17, val, count, 0.4f, 0.8f, 0.1f);
		gui::beginGroup();
		if (gui::Button(15, "OK2")) {
			LOG << "Int Test: " << _iTest;
			LOG << "Name: '" << _name << "'";
			LOG << "OK2 pressed";
		}
		if (gui::Button(16, "Cancel2")) {
			LOG << "Cancel2 clicked";
			LOG << "v2Test: " << DBG_V2(_v2Test);
		}
		gui::endGroup();
	}
	gui::end();
	PR_END("GUITest::render")
	*/
}


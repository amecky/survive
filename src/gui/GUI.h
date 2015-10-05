#pragma once
#include <Vector.h>
#include <vector>
#include <renderer\BitmapFont.h>

enum WindowState {
	WS_ACTIVE,
	WS_CLOSED
};

namespace gui {

	void initialize();

	void begin(const char* header,WindowState state = WS_ACTIVE);

	void Label(const char* text);

	bool Button(const char* label);

	void InputFloat(const char* label, float* v);

	void end();
}

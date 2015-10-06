#include "GUI.h"
#include <renderer\graphics.h>
#include <sprites\SpriteBatch.h>
#include <utils\font.h>

namespace gui {

	const float BUTTON_HEIGHT = 24.0f;
	const float TEXTFIELD_HEIGHT = 24.0f;
	const float TEXT_PADDING = 2.0f;
	const float LINE_HEIGHT = 26.0f;
	const float TEXT_OFFSET = 8.0f;
	const int CHAR_PADDING = 0;
	const float BOX_HEIGHT = 18.0f;

	struct GUID {
		int id;
		int index;
	};

	struct DrawCall {

		int type;
		char text[32];
		ds::Color color;
		v2 size;
		v2 position;
		ds::Texture texture;
		int padding;
	};

	struct GUIWindow {

		DrawCall calls[64];
		int num;
		v2 dim;
		v2 position;
		v2 startPosition;
		const char* header;

		void reset() {
			num = 0;
			dim = v2(0, 0);
		}

		void calculateDimension(const v2& position, const v2& size) {
			float endX = position.x + size.x - startPosition.x;
			float endY = startPosition.y - position.y - size.y;
			if (endX > dim.x) {
				dim.x = endX;
			}
			if (endY > dim.y) {
				dim.y = endY;
			}
		}

		void addBox(const v2& position,const v2& size, const ds::Color& color) {
			DrawCall& call = calls[num++];
			call.type = 1;
			call.color = color;
			call.size = size;
			call.position = position;
			calculateDimension(position,size);
			call.padding = 2;
		}

		void addBox(const v2& position, const v2& size, const ds::Texture& texture) {
			DrawCall& call = calls[num++];
			call.type = 3;
			call.color = ds::Color::WHITE;
			call.size = size;
			call.texture = texture;
			call.position = position;
			calculateDimension(position, size);
			call.padding = 2;

		}

		void addText(const v2& position,const char* text, const v2& size) {
			DrawCall& call = calls[num++];
			call.type = 2;
			call.color = ds::Color::WHITE;
			call.size = size;
			sprintf_s(call.text,32,text);
			call.position = position;
			call.position.y -= TEXT_OFFSET;
			calculateDimension(position, size);
			call.padding = CHAR_PADDING;
		}

		void nextPosition(bool grouped) {
			if (!grouped) {
				position.y -= LINE_HEIGHT;
			}
			else {
				if (num > 0) {
					position.x += calls[num-1].size.x + 50.0f;
				}
			}
		}
	};

	struct KeyInput {
		unsigned char keys[256];
		int num;
	};

	struct GUIContext {

		ds::BitmapFont* font;
		int textureID;
		GUIWindow window;
		GUIWindow popup;
		v2 cursorPosition;
		bool buttonPressed;
		bool clicked;
		bool released;
		int hot;
		int active;
		bool grouped;
		char inputText[32];
		KeyInput keyInput;
		bool visible;
		int caretPos;

		void nextPosition() {
			window.nextPosition(grouped);
		}
	};

	static GUIContext* guiContext;

	void sendKey(unsigned char c) {
		if (guiContext->keyInput.num < 256) {
			guiContext->keyInput.keys[guiContext->keyInput.num++] = c;
		}
	}
	
	// -------------------------------------------------------
	// check if mouse cursor is inside box
	// -------------------------------------------------------
	bool isCursorInside(const v2& p, const v2& dim) {
		v2 mp = guiContext->cursorPosition;
		if (mp.x < (p.x - dim.x * 0.5f)) {
			return false;
		}
		if (mp.x >(p.x + dim.x * 0.5f)) {
			return false;
		}
		if (mp.y < (p.y - dim.y * 0.5f)) {
			return false;
		}
		if (mp.y >(p.y + dim.y * 0.5f)) {
			return false;
		}
		return true;
	}

	// -------------------------------------------------------
	// begin group
	// -------------------------------------------------------
	void beginGroup() {
		guiContext->grouped = true;
	}

	// -------------------------------------------------------
	// end group
	// -------------------------------------------------------
	void endGroup() {
		guiContext->grouped = false;
	}

	// -------------------------------------------------------
	// check if widget is hot
	// -------------------------------------------------------
	bool isHot(int id, const v2& pos, const v2& size) {
		if (isCursorInside(pos, size)) {
			guiContext->hot = id;
			return true;
		}
		return false;
	}

	// -------------------------------------------------------
	// handle mouse interaction
	// -------------------------------------------------------
	bool handleMouse(int id, const v2& pos, const v2& size) {
		if (guiContext->active == id) {
			if (guiContext->clicked && guiContext->hot == id) {
				if (isCursorInside(pos, size)) {
					return true;
				}
				else {
					guiContext->active = -1;
				}
			}
		}
		else if (guiContext->hot == id) {
			if (guiContext->buttonPressed) {
				guiContext->keyInput.num = 0;
				guiContext->active = id;
			}
		}
		if (isCursorInside(pos, size)) {
			guiContext->hot = id;
		}
		return false;
	}
	// -------------------------------------------------------
	// begin panel
	// -------------------------------------------------------
	bool begin(const char* header,int* state,const v2& startPos) {
		ds::sprites::setTexture(guiContext->textureID);
		guiContext->window.position = startPos;
		guiContext->window.startPosition = startPos;
		guiContext->window.reset();
		guiContext->window.header = header;
		guiContext->cursorPosition = ds::renderer::getMousePosition();
		guiContext->clicked = false;
		guiContext->grouped = false;
		guiContext->visible = false;
		//guiContext->inputText[0] = '\0';
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			guiContext->buttonPressed = true;
		}
		else {
			if (guiContext->buttonPressed) {
				guiContext->clicked = true;
			}
			guiContext->buttonPressed = false;
		}
		// build panel header
		float width = 200.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f;
		bool active = handleMouse(0, p, v2(width, BOX_HEIGHT));
		if (active) {
			if (*state == 0) {
				*state = 1;
			}
			else {
				*state = 0;
			}
		}		
		if (*state == 1) {
			guiContext->visible = true;
		}
		guiContext->nextPosition();
		return *state == 1;
	}

	v2 getTextSize(const char* text) {
		return ds::font::calculateSize(*guiContext->font, text,CHAR_PADDING);
	}
	// -------------------------------------------------------
	// Label
	// -------------------------------------------------------
	void Label(const char* text) {
		v2 textDim = getTextSize(text);
		v2 p = guiContext->window.position;
		guiContext->window.addText(p, text, textDim);
		guiContext->nextPosition();
	}

	void handleTextInput() {
		int len = strlen(guiContext->inputText);
		if (guiContext->keyInput.num > 0) {
			for (int i = 0; i < guiContext->keyInput.num; ++i) {
				if (guiContext->keyInput.keys[i] == 128) {
					if (guiContext->caretPos > 0) {
						if (guiContext->caretPos < len) {
							memmove(guiContext->inputText + guiContext->caretPos - 1, guiContext->inputText + guiContext->caretPos, len - guiContext->caretPos);
						}
						--guiContext->caretPos;
						--len;
						guiContext->inputText[len] = '\0';
						
					}
				}
				else if (guiContext->keyInput.keys[i] == 133) {
					guiContext->caretPos = 0;
				}
				else if (guiContext->keyInput.keys[i] == 129) {
					if (guiContext->caretPos > 0) {
						--guiContext->caretPos;
					}
				}
				else if (guiContext->keyInput.keys[i] == 130) {
					if (guiContext->caretPos < strlen(guiContext->inputText)) {
						++guiContext->caretPos;
					}
				}
				else if (guiContext->keyInput.keys[i] == 132) {
					guiContext->caretPos = strlen(guiContext->inputText);
				}
				else if (guiContext->keyInput.keys[i] == 131) {
					guiContext->active = -1;
				}
				else if (guiContext->keyInput.keys[i] > 10 && guiContext->keyInput.keys[i] < 128) {
					if (len < 32) {
						if (guiContext->caretPos < len) {
							memmove(guiContext->inputText + guiContext->caretPos + 1, guiContext->inputText + guiContext->caretPos, len - guiContext->caretPos);
						}
						guiContext->inputText[guiContext->caretPos] = guiContext->keyInput.keys[i];
						++len;
						++guiContext->caretPos;
					}
				}
			}
			++len;
			guiContext->inputText[len] = '\0';
			guiContext->keyInput.num = 0;
		}
	}

	// -------------------------------------------------------
	// input float
	// -------------------------------------------------------
	void InputFloat(int id,const char* label, float* v) {
		v2 textDim = getTextSize(label);
		float width = 100.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f;
		int prev = guiContext->active;
		bool active = handleMouse(id, p, v2(width, BOX_HEIGHT));
		if (prev != guiContext->active) {
			sprintf_s(guiContext->inputText, 32, "%.2f", *v);
			guiContext->caretPos = strlen(guiContext->inputText);
		}
		if (guiContext->active == id) {
			guiContext->window.addBox(p, v2(width, BOX_HEIGHT), ds::Color(64, 64, 64, 255));
			handleTextInput();
			p.x = guiContext->window.position.x + 8.0f;
			textDim = getTextSize(guiContext->inputText);
			guiContext->window.addText(p, guiContext->inputText, textDim);
			*v = atof(guiContext->inputText);
			v2 cp = p;
			v2 cursorPos = ds::font::calculateLimitedSize(*guiContext->font, guiContext->inputText, guiContext->caretPos, CHAR_PADDING);
			cp.x = guiContext->window.position.x + 4.0f + cursorPos.x;
			cp.y -= 2.0f;
			guiContext->window.addBox(cp, v2(2, 18.0f), ds::Color(192, 0, 0, 255));
		}
		else {
			char buffer[32];
			sprintf_s(buffer, 32, "%.2f", *v);
			guiContext->window.addBox(p, v2(width, BOX_HEIGHT), ds::Color(128, 128, 128, 255));
			p.x = guiContext->window.position.x + 8.0f;
			textDim = getTextSize(buffer);
			guiContext->window.addText(p, buffer, textDim);
		}		
		p.x += 110.0f;
		textDim = getTextSize(label);
		guiContext->window.addText(p, label, textDim);
		guiContext->nextPosition();
	}

	// -------------------------------------------------------
	// input scalar
	// -------------------------------------------------------
	void InputScalar(int id,int index, int* v) {
		int new_id = id + 1024 * index;
		float width = 100.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f + 110.0f * index;
		//sprintf_s(guiContext->inputText, 32, "%d", *v);
		int prev = guiContext->active;
		bool active = handleMouse(new_id, p, v2(width, BOX_HEIGHT));
		if (prev != guiContext->active) {
			sprintf_s(guiContext->inputText, 32, "%d", *v);
			guiContext->caretPos = strlen(guiContext->inputText);
		}
		if (guiContext->active == new_id) {
			guiContext->window.addBox(p, v2(width, BOX_HEIGHT), ds::Color(64, 64, 64, 255));
			handleTextInput();
			*v = atoi(guiContext->inputText);
			v2 cp = p;
			v2 cursorPos = ds::font::calculateLimitedSize(*guiContext->font, guiContext->inputText,guiContext->caretPos,CHAR_PADDING);
			cp.x = guiContext->window.position.x + 4.0f + 110.0f * index + cursorPos.x;
			cp.y -= 2.0f;
			guiContext->window.addBox(cp, v2(2, 18.0f), ds::Color(192, 0, 0, 255));
			p.x = guiContext->window.position.x + 8.0f + 110.0f * index;
			v2 textDim = getTextSize(guiContext->inputText);
			guiContext->window.addText(p, guiContext->inputText, textDim);
		}
		else {
			char buffer[32];
			sprintf_s(buffer, 32, "%d", *v);
			guiContext->window.addBox(p, v2(width, BOX_HEIGHT), ds::Color(128, 128, 128, 255));
			p.x = guiContext->window.position.x + 8.0f + 110.0f * index;
			v2 textDim = getTextSize(buffer);
			guiContext->window.addText(p, buffer, textDim);
		}		
	}

	// -------------------------------------------------------
	// input int
	// -------------------------------------------------------
	void InputInt(int id, const char* label, int* v) {
		InputScalar(id, 0, v);
		v2 p = guiContext->window.position;
		p.x += 110.0f;
		v2 textDim = getTextSize(label);
		guiContext->window.addText(p, label, textDim);
		guiContext->nextPosition();
	}

	// -------------------------------------------------------
	// input v2
	// -------------------------------------------------------
	void InputVec2(int id, const char* label, v2* v) {
		int x = v->x;
		int y = v->y;
		InputScalar(id, 0, &x);
		InputScalar(id, 1, &y);	
		v->x = x;
		v->y = y;
		v2 p = guiContext->window.position;
		p.x += 220.0f;
		v2 textDim = ds::font::calculateSize(*guiContext->font, label);
		guiContext->window.addText(p, label, textDim);
		guiContext->nextPosition();
	}

	// -------------------------------------------------------
	// input vec3
	// -------------------------------------------------------
	void InputVec3(int id, const char* label, v3* v) {
		int x = v->x;
		int y = v->y;
		int z = v->z;
		InputScalar(id, 0, &x);
		InputScalar(id, 1, &y);
		InputScalar(id, 2, &z);
		v->x = x;
		v->y = y;
		v->z = z;
		v2 p = guiContext->window.position;
		p.x += 330.0f;
		v2 textDim = getTextSize(label);
		guiContext->window.addText(p, label, textDim);
		guiContext->nextPosition();
	}

	// -------------------------------------------------------
	// input rect
	// -------------------------------------------------------
	void InputRect(int id, const char* label, ds::Rect* v) {
		int top = v->top;
		int left = v->left;
		int width = v->width();
		int height = v->height();
		InputScalar(id, 0, &top);
		InputScalar(id, 1, &left);
		InputScalar(id, 2, &width);
		InputScalar(id, 3, &height);
		*v = ds::Rect(top, left, width, height);
		v2 p = guiContext->window.position;
		p.x += 440.0f;
		v2 textDim = getTextSize(label);
		guiContext->window.addText(p, label, textDim);
		guiContext->nextPosition();
	}

	// -------------------------------------------------------
	// combo box
	// -------------------------------------------------------	
	void ComboBox(int id, const std::vector<std::string>& entries, int* selected) {
		int max = entries.size();
		float width = 300.0f + 40.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f;		
		float height = max * BOX_HEIGHT;
		p.y -= height / 2.0f - BOX_HEIGHT / 2.0f;
		bool hot = isHot(id, p, v2(width, height));
		if (hot) {
			guiContext->window.addBox(p, v2(width, height), ds::Color(64, 64, 0, 255));
		}
		else {
			guiContext->window.addBox(p, v2(width, height), ds::Color(32, 32, 0, 255));
		}
		bool inside = isCursorInside(p, v2(width, height));
		p = guiContext->window.position;
		for (size_t i = 0; i < max; ++i) {
			p.x = guiContext->window.position.x + 10.0f;
			if (*selected == i) {
				v2 selp = p;
				selp.x += width / 2.0f;
				selp.x -= 10.0f;
				guiContext->window.addBox(selp, v2(width, BOX_HEIGHT), ds::Color(128, 128, 128, 255));
			}				
			v2 textDim = ds::font::calculateSize(*guiContext->font, entries[i].c_str());
			guiContext->window.addText(p, entries[i].c_str(), textDim);
			if (guiContext->clicked && isCursorInside(p, v2(width, 24.0f))) {
				*selected = i;
			}
			p.y -= BOX_HEIGHT;
		}
		guiContext->window.position.y -= height;
		guiContext->window.position.y -= 8.0f;
		//for (int i = 0; i < max ; ++i) {
			//guiContext->nextPosition();
		//}
	}
	
	// -------------------------------------------------------
	// button
	// -------------------------------------------------------	
	bool Button(int id,const char* label) {
		// get text size
		v2 textDim = getTextSize(label);
		float width = textDim.x + 40.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f;
		bool hot = isHot(id, p, v2(width, BUTTON_HEIGHT));
		if (hot) {
			guiContext->window.addBox(p, v2(width, BUTTON_HEIGHT), ds::Color(0, 64, 64, 255));
		}
		else {
			guiContext->window.addBox(p, v2(width, BUTTON_HEIGHT), ds::Color(0, 64, 0, 255));
		}
		bool inside = isCursorInside(p, v2(width, BUTTON_HEIGHT));
		p.x = guiContext->window.position.x + (width - textDim.x) / 2.0f;
		guiContext->window.addText(p,label, textDim);
		guiContext->nextPosition();
		return handleMouse(id, p, v2(width, BUTTON_HEIGHT));
	}

	// -------------------------------------------------------
	// intialize gui
	// -------------------------------------------------------	
	void initialize() {
		guiContext = new GUIContext;
		guiContext->textureID = ds::renderer::loadTexture("gui");
		assert(guiContext->textureID != -1);
		guiContext->font = ds::renderer::createBitmapFont("tahoma_12");
		ds::assets::load("tahoma_12", guiContext->font, ds::CVT_FONT);
		ds::renderer::initializeBitmapFont(*guiContext->font, guiContext->textureID);
		guiContext->clicked = false;
		guiContext->released = false;
		guiContext->buttonPressed = false;
		guiContext->active = -1;
		guiContext->hot = -1;
	}

	// -------------------------------------------------------
	// end panel
	// -------------------------------------------------------	
	void end() {
		// get dimension of entire panel
		v2 dim = guiContext->window.dim;
		if (dim.x == 0.0f) {
			dim.x = 200.0f;
		}
		dim += v2(20, 20);
		// draw header
		v2 p = guiContext->window.startPosition;
		float sx = 1.0f;
		if (dim.x > 200.0f) {
			sx = dim.x / 200.0f;
		}
		p.x = guiContext->window.startPosition.x + dim.x / 2.0f * sx - 10.0f;
		ds::sprites::draw(p, ds::math::buildTexture(200.0f, 0.0f, dim.x, 30.0f, 512.0f, 512.0f), 0.0f, sx, 1.0f, ds::Color(192, 0, 0, 255));
		p.y -= 7.0f;
		p.x = guiContext->window.startPosition.x + 10.0f;
		ds::sprites::drawText(guiContext->font, p.x, p.y, guiContext->window.header, 2);

		if (guiContext->window.num > 0) {			
			if (guiContext->visible) {
				v2 p = guiContext->window.startPosition;								
				sx = 1.0f;
				if (dim.x > 200.0f) {
					sx = dim.x / 200.0f;
				}
				float sy = 1.0f;
				if (dim.y > 200.0f) {
					sy = dim.y / 200.0f;
				}
				v2 center;
				center.x = guiContext->window.startPosition.x + dim.x / 2.0f * sx - 10.0f;
				center.y = guiContext->window.startPosition.y - dim.y / 2.0f * sy - 10.0f;
				ds::sprites::draw(center, ds::math::buildTexture(200.0f, 0.0f, dim.x, dim.y, 512.0f, 512.0f), 0.0f, sx, sy, ds::Color(16, 16, 16, 255));
			}
			for (int i = 0; i < guiContext->window.num; ++i) {
				const DrawCall& call = guiContext->window.calls[i];
				if (call.type == 1) {
					ds::sprites::draw(call.position, ds::math::buildTexture(200.0f, 0.0f, call.size.x, call.size.y, 512.0f, 512.0f), 0.0f, 1.0f, 1.0f, call.color);
				}
				else if (call.type == 2) {
					ds::sprites::drawText(guiContext->font, call.position.x, call.position.y, call.text,call.padding);
				}
				else if (call.type == 3) {
					ds::sprites::draw(call.position, call.texture, 0.0f, 1.0f, 1.0f, call.color);
				}
			}
		}
		if (guiContext->hot == -1 && guiContext->clicked) {
			guiContext->active = -1;
		}
	}

}

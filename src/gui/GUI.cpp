#include "GUI.h"
#include <renderer\graphics.h>
#include <sprites\SpriteBatch.h>
#include <utils\font.h>

namespace gui {

	struct GUID {
		int id;
		int index;
	};

	struct DrawCall {

		int type;
		const char* text;
		ds::Color color;
		v2 size;
		v2 position;
		int padding;
	};

	struct GUIWindow {

		DrawCall calls[64];
		int num;
		v2 dim;
		v2 position;

		void reset() {
			num = 0;
			dim = v2(0, 1000);
		}

		void addBox(const v2& position,const v2& size, const ds::Color& color) {
			DrawCall& call = calls[num++];
			call.type = 1;
			call.color = color;
			call.size = size;
			call.position = position;
			if (size.x > dim.x) {
				dim.x = size.x;
			}
			dim.y += size.y;
			call.padding = 2;

		}

		void addText(const v2& position,const char* text, const v2& size) {
			DrawCall& call = calls[num++];
			call.type = 2;
			call.color = ds::Color::WHITE;
			call.size = size;
			call.text = text;
			call.position = position;
			if (size.x > dim.x) {
				dim.x = size.x;
			}
			dim.y += size.y;
			call.padding = 2;
		}

		void nextPosition(bool grouped) {
			if (!grouped) {
				position.y -= 35.0f;
			}
			else {
				if (num > 0) {
					position.x += calls[num-1].size.x + 50.0f;
				}
			}
		}
	};

	struct GUIContext {

		ds::BitmapFont* font;
		int textureID;
		GUIWindow window;
		v2 cursorPosition;
		bool buttonPressed;
		bool clicked;
		bool released;
		int hot;
		int active;
		bool grouped;
		char inputText[32];

		void nextPosition() {
			window.nextPosition(grouped);
		}
	};

	static GUIContext* guiContext;


	
	// -------------------------------------------------------
	// begin panel
	// -------------------------------------------------------
	void begin(const char* header,WindowState state) {
		ds::sprites::setTexture(guiContext->textureID);
		guiContext->window.position = v2(10, 750);
		guiContext->window.reset();
		guiContext->cursorPosition = ds::renderer::getMousePosition();
		guiContext->clicked = false;
		guiContext->grouped = false;
		guiContext->inputText[0] = '\0';
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
		v2 textDim = ds::font::calculateSize(*guiContext->font, header);
		float width = 200.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f;
		guiContext->window.addBox(p, v2(200.0f, 30.0f), ds::Color(192, 0, 0, 255));
		p.y -= 7.0f;
		p.x = guiContext->window.position.x + (width - textDim.x) / 2.0f;
		guiContext->window.addText(p,header, textDim);		
		guiContext->nextPosition();
	}

	void Label(const char* text) {
		v2 textDim = ds::font::calculateSize(*guiContext->font, text);
		guiContext->window.addText(guiContext->window.position, text, textDim);
		guiContext->nextPosition();
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

	void beginGroup() {
		guiContext->grouped = true;
	}

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
				guiContext->active = id;
			}
		}		
		if (isCursorInside(pos, size)) {
			guiContext->hot = id;
		}
		return false;
	}

	

	void InputFloat(int id,const char* label, float* v) {
		v2 textDim = ds::font::calculateSize(*guiContext->font, label);
		float width = 100.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f;
		char buffer[32];
		sprintf_s(buffer, 32, "%.2f", *v);
		bool active = handleMouse(id, p, v2(width, 24.0f));
		if (guiContext->active == id) {
			guiContext->window.addBox(p, v2(width, 24.0f), ds::Color(64, 64, 64, 255));
			// https://code.google.com/p/nvidia-widgets/source/browse/trunk/src/nvwidgets/nvWidgets.cpp
			sprintf_s(guiContext->inputText, 32, "%.2f", *v);
		}
		else {
			guiContext->window.addBox(p, v2(width, 24.0f), ds::Color(128, 128, 128, 255));
		}
		p.y -= 8.0f;
		p.x = guiContext->window.position.x + 8.0f;
		
		textDim = ds::font::calculateSize(*guiContext->font, label);
		guiContext->window.addText(p, buffer, textDim);
		p.x += 110.0f;
		guiContext->window.addText(p, label, textDim);
		guiContext->nextPosition();
	}

	
	// -------------------------------------------------------
	// button
	// -------------------------------------------------------	
	bool Button(int id,const char* label) {
		// get text size
		v2 textDim = ds::font::calculateSize(*guiContext->font, label);
		float width = textDim.x + 40.0f;
		v2 p = guiContext->window.position;
		p.x += width / 2.0f;
		bool hot = isHot(id,p, v2(width, 24.0f));
		if (hot) {
			guiContext->window.addBox(p, v2(width, 30.0f), ds::Color(0, 64, 64, 255));
		}
		else {
			guiContext->window.addBox(p, v2(width, 30.0f), ds::Color(0, 64, 0, 255));
		}
		bool inside = isCursorInside(p, v2(width, 30.0f));
		p.y -= 7.0f;
		p.x = guiContext->window.position.x + (width - textDim.x) / 2.0f;
		guiContext->window.addText(p,label, textDim);
		guiContext->nextPosition();
		return handleMouse(id, p, v2(width, 24.0f));
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
	// end paenl
	// -------------------------------------------------------	
	void end() {
		if (guiContext->window.num > 0) {
			v2 p = v2(10, 720);
			v2 dim = guiContext->window.dim;
			dim += v2(20, 20);
			float sx = 1.0f;
			if (dim.x > 200.0f) {
				sx = dim.x / 200.0f;
			}
			v2 center;
			center.x = 10.0f + dim.x / 2.0f * sx - 10.0f;
			center.y = 720.0f - dim.y / 2.0f + 10.0f;
			
			//LOG << "dim: " << DBG_V2(dim) << " center: " << DBG_V2(center);
			ds::sprites::draw(center, ds::math::buildTexture(200.0f, 0.0f, dim.x, dim.y, 512.0f, 512.0f), 0.0f, sx, 1.0f, ds::Color(16,16,16,255));
			for (int i = 0; i < guiContext->window.num; ++i) {
				const DrawCall& call = guiContext->window.calls[i];
				if (call.type == 1) {
					ds::sprites::draw(call.position, ds::math::buildTexture(200.0f, 0.0f, call.size.x, call.size.y, 512.0f, 512.0f), 0.0f, 1.0f, 1.0f, call.color);
				}
				else if (call.type == 2) {
					ds::sprites::drawText(guiContext->font, call.position.x, call.position.y, call.text,call.padding);
				}
			}
		}
		if (guiContext->hot == -1 && guiContext->clicked) {
			guiContext->active = -1;
		}
	}

}

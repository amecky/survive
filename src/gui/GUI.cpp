#include "GUI.h"
#include <renderer\graphics.h>
#include <sprites\SpriteBatch.h>
#include <utils\font.h>

namespace gui {

	struct GUIState {
		v2 position;
	};

	static GUIState* guiState;

	struct DrawCall {

		int type;
		const char* text;
		ds::Color color;
		v2 size;
		v2 position;
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
		}
	};

	struct GUIContext {

		ds::BitmapFont* font;
		int textureID;
		GUIWindow window;
		v2 cursorPosition;
		bool buttonPressed;
		bool clicked;
	};

	static GUIContext* guiContext;


	

	void begin(const char* header,WindowState state) {
		ds::sprites::setTexture(guiContext->textureID);
		guiState->position = v2(10, 750);
		guiContext->window.reset();
		guiContext->cursorPosition = ds::renderer::getMousePosition();

		guiContext->clicked = false;
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			guiContext->buttonPressed = true;
		}
		else {
			if (guiContext->buttonPressed) {
				LOG << "clicked at " << DBG_V2(guiContext->cursorPosition);
				guiContext->clicked = true;
			}
			guiContext->buttonPressed = false;
		}
		v2 textDim = ds::font::calculateSize(*guiContext->font, header);
		float width = 200.0f;
		v2 p = guiState->position;
		p.x += width / 2.0f;
		guiContext->window.addBox(p, v2(200.0f, 30.0f), ds::Color(192, 0, 0, 255));
		p.y -= 7.0f;
		p.x = guiState->position.x + (width - textDim.x) / 2.0f;
		guiState->position.y -= 35.0f;
		
		
		guiContext->window.addText(p,header, textDim);
	}

	void Label(const char* text) {
		v2 textDim = ds::font::calculateSize(*guiContext->font, text);
		guiContext->window.addText(guiState->position,text, textDim);
		guiState->position.y -= 30.0f;
	}

	bool isCursorInside(const v2& p, const v2& dim) {
		v2 mp = guiContext->cursorPosition;
		if (mp.x < (p.x - dim.x * 0.5f)) {
			return false;
		}
		if (mp.x > (p.x + dim.x * 0.5f)) {
			return false;
		}
		if (mp.y < (p.y - dim.y * 0.5f)) {
			return false;
		}
		if (mp.y > (p.y + dim.y * 0.5f)) {
			return false;
		}
		return true;
	}

	void InputFloat(const char* label, float* v) {
		v2 textDim = ds::font::calculateSize(*guiContext->font, label);
		float width = 100.0f;
		v2 p = guiState->position;
		p.x += width / 2.0f;
		guiContext->window.addBox(p, v2(width, 30.0f), ds::Color(64, 64, 64, 255));
		p.y -= 7.0f;
		p.x = guiState->position.x + (width - textDim.x) / 2.0f;
		char buffer[32];
		sprintf_s(buffer, 32, "%.2f", *v);
		textDim = ds::font::calculateSize(*guiContext->font, label);
		guiContext->window.addText(p, buffer, textDim);
		p.x += 140.0f;
		guiContext->window.addText(p, label, textDim);
		guiState->position.y -= 35.0f;
	}

	bool Button(const char* label) {
		// get text size
		v2 textDim = ds::font::calculateSize(*guiContext->font, label);
		float width = textDim.x + 40.0f;
		v2 p = guiState->position;
		p.x += width / 2.0f;
		guiContext->window.addBox(p,v2(width, 30.0f), ds::Color(64, 64, 64, 255));
		bool inside = isCursorInside(p, v2(width, 30.0f));
		p.y -= 7.0f;
		p.x = guiState->position.x + (width - textDim.x) / 2.0f;
		guiContext->window.addText(p,label, textDim);
		guiState->position.y -= 35.0f;
		if (guiContext->clicked) {
			return inside;
		}
		return false;
	}

	void initialize() {
		guiContext = new GUIContext;
		guiContext->textureID = ds::renderer::loadTexture("gui");
		assert(guiContext->textureID != -1);
		guiContext->font = ds::renderer::createBitmapFont("tahoma_12");
		ds::assets::load("tahoma_12", guiContext->font, ds::CVT_FONT);
		ds::renderer::initializeBitmapFont(*guiContext->font, guiContext->textureID);
		guiState = new GUIState;
		guiState->position = v2(10, 720);
	}

	void end() {
		if (guiContext->window.num > 0) {
			v2 p = v2(10, 720);
			v2 dim = guiContext->window.dim;
			float sx = 1.0f;
			if (dim.x > 200.0f) {
				sx = dim.x / 200.0f;
			}
			v2 center;
			center.x = 10.0f + dim.x / 2.0f * sx;
			center.y = 720.0f - dim.y / 2.0f;
			
			//LOG << "dim: " << DBG_V2(dim) << " center: " << DBG_V2(center);
			ds::sprites::draw(center, ds::math::buildTexture(200.0f, 0.0f, dim.x, dim.y, 512.0f, 512.0f), 0.0f, sx, 1.0f, ds::Color(16,16,16,255));
			for (int i = 0; i < guiContext->window.num; ++i) {
				const DrawCall& call = guiContext->window.calls[i];
				if (call.type == 1) {
					ds::sprites::draw(call.position, ds::math::buildTexture(200.0f, 0.0f, call.size.x, call.size.y, 512.0f, 512.0f), 0.0f, 1.0f, 1.0f, call.color);
				}
				else if (call.type == 2) {
					ds::sprites::drawText(guiContext->font, call.position.x, call.position.y, call.text);
				}
			}
		}
	}

}

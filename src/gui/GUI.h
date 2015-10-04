#pragma once
#include <Vector.h>
#include <vector>
#include <renderer\BitmapFont.h>

struct GUIContext {

	ds::BitmapFont* font;
	int textureID;
};

class Widget {

public:
	Widget(GUIContext* context,const v2& pos) : _context(context) , _position(pos), _active(true), _size(100, 100), _parent(0) {}
	virtual ~Widget() {}
	virtual void render() = 0;
	const v2& getPosition() const {
		return _position;
	}
	void setParent(Widget* w) {
		_parent = w;
	}
protected:
	void draw_box(const v2& pos, const v2& size);
	v2 _position;
	v2 _size;
	Widget* _parent;
	GUIContext* _context;
private:
	bool _active;
};

class PanelWidget : public Widget {

public:
	PanelWidget(GUIContext* context, const v2& pos) : Widget(context,pos) {}
	~PanelWidget() {}
	void render();
};

class LabelWidget : public Widget {

public:
	LabelWidget(GUIContext* context, const v2& pos, const std::string& txt) : Widget(context, pos), _text(txt) {}
	~LabelWidget() {}
	void render();
private:
	std::string _text;
};

class ButtonWidget : public Widget {

public:
	ButtonWidget(GUIContext* context, const v2& pos) : Widget(context, pos) {}
	~ButtonWidget() {}
	void render() {}
};

class GUI {

typedef std::vector<Widget*> Widgets;

public:
	GUI();
	~GUI();
	void intialize();
	PanelWidget* addPanel(const v2& pos, const v2& size);
	LabelWidget* addLabel(const v2& pos, const std::string& txt,Widget* parent);
	void render();
private:
	Widgets _widgets;
	GUIContext* _context;
};


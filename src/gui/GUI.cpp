#include "GUI.h"
#include <renderer\graphics.h>
#include <sprites\SpriteBatch.h>

void Widget::draw_box(const v2& pos, const v2& size) {
	v2 p = pos;
	if (_parent != 0) {
		p += _parent->getPosition();
	}
	ds::sprites::draw(p, ds::math::buildTexture(0.0f, 0.0f, size.x, size.y),0.0f,1.0f,1.0f,ds::Color(64,64,64,255));
}

void PanelWidget::render() {
	draw_box(_position, _size);
}

void LabelWidget::render() {
	v2 p = _position;
	if (_parent != 0) {
		p += _parent->getPosition();
	}
	ds::sprites::drawText(_context->font, p.x, p.y, _text.c_str());
}

GUI::GUI() {
	_context = new GUIContext;
}


GUI::~GUI() {
	for (size_t i = 0; i < _widgets.size(); ++i) {
		delete _widgets[i];
	}
}

void GUI::intialize() {
	_context->textureID = ds::renderer::loadTexture("gui");
	assert(_context->textureID != -1);
	_context->font = ds::renderer::createBitmapFont("tahoma_12");
	ds::assets::load("tahoma_12", _context->font, ds::CVT_FONT);
	ds::renderer::initializeBitmapFont(*_context->font, _context->textureID);
}

LabelWidget* GUI::addLabel(const v2& pos, const std::string& txt,Widget* parent) {
	LabelWidget* w = new LabelWidget(_context, pos,txt);
	w->setParent(parent);
	_widgets.push_back(w);
	return w;

}
PanelWidget* GUI::addPanel(const v2& p, const v2& s) {
	PanelWidget* w = new PanelWidget(_context,p);
	_widgets.push_back(w);
	return w;
}

void GUI::render() {
	ds::sprites::setTexture(_context->textureID);
	for (size_t i = 0; i < _widgets.size(); ++i) {
		_widgets[i]->render();
	}
}
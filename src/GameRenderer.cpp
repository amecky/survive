#include "GameRenderer.h"
#include "Constants.h"

GameRenderer::GameRenderer(GameContext* context) : _context(context) {
	//_context->viewport_id = ds::renderer::createViewport(1280, 720, 1600, 900);
	//ds::renderer::setViewportPosition(_context->viewport_id, v2(800, 450));

	//_context->world->attach_descriptor(EFFECT_LAYER, "background_gradient");
	//_context->world->create(v2(640, 360), ds::math::buildTexture(0, 0, 1280, 720), 0.0f, 1.0f, 1.0f, ds::Color::WHITE, 33, EFFECT_LAYER);
	/*
	m_AddBS = ds::renderer::createBlendState("alpha_blend_state", ds::BL_ONE, ds::BL_ONE, true);
	
	_rt1 = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 0));
	_rt2 = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 0));
	//snakes = new Snake(&m_Context);

	ds::Descriptor light_desc;
	light_desc.shader = ds::renderer::loadShader("lightning", "LTTech");
	assert(light_desc.shader != 0);
	light_desc.texture = 0;
	_light_desc = ds::renderer::addDescriptor(light_desc);
	

	_shader = ds::renderer::getShader(light_desc.shader);
	_shader->setTexture("gTex", _rt2.textureID);
	_shader->setTexture("gBackTex", _rt1.textureID);
	*/

	//int sh = ds::renderer::loadShader("background", "BackgroundTech");
	//assert(sh != 0);
	//_shader = ds::renderer::getShader("background");
	//assert(_shader != 0);
	_border_color = ds::Color(64, 64, 64, 255);
	createBorder();
}


GameRenderer::~GameRenderer() {
}

void GameRenderer::hitBorder(const v2& pos) {
	int x = (pos.x - 30) / 40;
	int y = (pos.y - 30) / 40;
	int index = x + y * 30;
	for (int i = 0; i < _borderTiles.size(); ++i) {
		v2 d = _borderTiles[i].position - pos;
		if (sqr_length(d) < 40.0f * 40.0f) {
			LOG << "found!";
			_context->world->flashColor(_borderTiles[i].sid, _border_color, ds::Color(192, 0, 0, 255),  0.4f);
		}
	}
}

v2 convertFromGrid(int x, int y) {
	v2 p;
	p.x = 30.0f + x * 40.0f;
	p.y = 30.0f + y * 40.0f;
	return p;
}
// -------------------------------------------------------
// draw border
// -------------------------------------------------------
void GameRenderer::createBorder() {
	// background
	//_context->world->create(v2(480, 306), ds::math::buildTexture(0, 512, 480, 306), 0.0f, 2.0f, 2.0f, ds::Color::WHITE,32,BG_LAYER);
	//_context->world->create(v2(1280, 306), ds::math::buildTexture(0, 512, 320, 306), 0.0f, 2.0f, 2.0f, ds::Color::WHITE, 32, BG_LAYER);
	//_context->world->create(v2(480, 756), ds::math::buildTexture(0, 512, 480, 144), 0.0f, 2.0f, 2.0f, ds::Color::WHITE, 32, BG_LAYER);
	//_context->world->create(v2(1280, 756), ds::math::buildTexture(0, 512, 320, 144), 0.0f, 2.0f, 2.0f, ds::Color::WHITE, 32, BG_LAYER);
	// 4 corners
	//_context->world->create(v2(40, 860), ds::math::buildTexture(840, 0, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	//_context->world->create(v2(40, 40), ds::math::buildTexture(940, 0, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	//_context->world->create(v2(1560, 860), ds::math::buildTexture(840, 280, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	//_context->world->create(v2(1560, 40), ds::math::buildTexture(940, 280, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	// left and right wall
	for (int i = 0; i < 15; ++i) {
		BorderTile tl;
		v2 p = v2(30, 60 + i * 40);
		tl.sid = _context->world->create(p, ds::math::buildTexture(440, 20, 6, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
		tl.position = p;
		_borderTiles.push_back(tl);
		BorderTile tr;
		p = v2(1250, 60 + i * 40);
		tr.sid = _context->world->create(p, ds::math::buildTexture(440, 80, 6, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
		tr.position = p;
		_borderTiles.push_back(tr);
	}
	// bottom and top wall
	for (int i = 0; i < 30; ++i) {
		BorderTile tt;
		v2 p = v2(60 + i * 40, 650);
		tt.sid = _context->world->create(p, ds::math::buildTexture(440, 40, 40, 6), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
		tt.position = p;
		_borderTiles.push_back(tt);
		BorderTile tb;
		p = v2(60 + i * 40,30);
		tb.sid = _context->world->create(p, ds::math::buildTexture(440, 40, 40, 6), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
		tb.position = p;
		_borderTiles.push_back(tb);
	}
	// missing left and right pieces
	//_context->world->create(v2(40, 810), ds::math::buildTexture(880, 0, 40, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	//_context->world->create(v2(1560, 810), ds::math::buildTexture(880, 280, 40, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	// missing top and bottom pieces
	//_context->world->create(v2(1505, 870), ds::math::buildTexture(840, 40, 90, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	//_context->world->create(v2(1505, 30), ds::math::buildTexture(960, 40, 90, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
}


// -------------------------------------------------------
// draw border
// -------------------------------------------------------
void GameRenderer::drawBorder() {
}

void GameRenderer::renderWorld() {
	ZoneTracker z("GameRenderer:render");
	//ds::renderer::selectViewport(_context->viewport_id);
	//_context->world->renderSingleLayer(EFFECT_LAYER);
	//ds::renderer::draw_screen_quad(_shader);
	//ds::renderer::setRenderTarget(_rt1);	
	_context->world->renderSingleLayer(BG_LAYER);
	//ds::renderer::setRenderTarget(_rt2);
	//_context->world->renderSingleLayer(LIGHT_LAYER);
	//ds::renderer::restoreBackBuffer();
	//ds::sprites::flush();
//	ds::sprites::flush();
	//_context->world->renderSingleLayer(MESSAGE_LAYER);	
	//ds::sprites::flush();
	//ds::renderer::selectViewport(_context->viewport_id);
	_context->particles->render();
	//ds::renderer::selectViewport(_context->viewport_id);
	_context->world->renderSingleLayer(OBJECT_LAYER);
	ds::sprites::flush();
	//ds::renderer::selectViewport(0);
}
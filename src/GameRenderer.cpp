#include "GameRenderer.h"
#include "Constants.h"

GameRenderer::GameRenderer(GameContext* context) : _context(context) {
	_context->viewport_id = ds::renderer::createViewport(1280, 720, 1600, 900);
	ds::renderer::setViewportPosition(_context->viewport_id, v2(800, 450));

	_context->world->attachViewport(BG_LAYER, _context->viewport_id);
	_context->world->attachViewport(LIGHT_LAYER, _context->viewport_id);
	_context->world->attachViewport(OBJECT_LAYER, _context->viewport_id);
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
	_border_color = ds::Color(255, 122, 225, 255);
	createBorder();
}


GameRenderer::~GameRenderer() {
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
	_context->world->create(v2(40, 860), ds::math::buildTexture(840, 0, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	_context->world->create(v2(40, 40), ds::math::buildTexture(940, 0, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	_context->world->create(v2(1560, 860), ds::math::buildTexture(840, 280, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	_context->world->create(v2(1560, 40), ds::math::buildTexture(940, 280, 40, 60), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	// left and right wall
	for (int i = 0; i < 9; ++i) {
		_context->world->create(v2(40, 110 + i * 80), ds::math::buildTexture(880, 0, 40, 80), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
		_context->world->create(v2(1560, 110 + i * 80), ds::math::buildTexture(880, 280, 40, 80), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	}
	// bottom and top wall
	for (int i = 0; i < 7; ++i) {
		_context->world->create(v2(160 + i * 200, 870), ds::math::buildTexture(840, 40, 200, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
		_context->world->create(v2(160 + i * 200, 30), ds::math::buildTexture(960, 40, 200, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	}
	// missing left and right pieces
	_context->world->create(v2(40, 810), ds::math::buildTexture(880, 0, 40, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	_context->world->create(v2(1560, 810), ds::math::buildTexture(880, 280, 40, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	// missing top and bottom pieces
	_context->world->create(v2(1505, 870), ds::math::buildTexture(840, 40, 90, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
	_context->world->create(v2(1505, 30), ds::math::buildTexture(960, 40, 90, 40), 0.0f, 1.0f, 1.0f, _border_color, 32, BG_LAYER);
}


// -------------------------------------------------------
// draw border
// -------------------------------------------------------
void GameRenderer::drawBorder() {
}

void GameRenderer::renderWorld() {
	ZoneTracker z("GameRenderer:render");
	//_context->world->renderSingleLayer(EFFECT_LAYER);
	//ds::renderer::draw_screen_quad(_shader);
	//ds::renderer::setRenderTarget(_rt1);	
	_context->world->renderSingleLayer(BG_LAYER);
	//ds::renderer::setRenderTarget(_rt2);
	//_context->world->renderSingleLayer(LIGHT_LAYER);
	//ds::renderer::restoreBackBuffer();
	//ds::sprites::flush();
	ds::sprites::flush();
	_context->world->renderSingleLayer(MESSAGE_LAYER);	
	ds::sprites::flush();
	ds::renderer::selectViewport(_context->viewport_id);
	_context->particles->render();
	ds::renderer::selectViewport(_context->viewport_id);
	_context->world->renderSingleLayer(OBJECT_LAYER);
	ds::renderer::selectViewport(0);
}
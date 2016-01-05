#include "GameRenderer.h"
#include "Constants.h"

GameRenderer::GameRenderer(GameContext* context) : _context(context) {
	_context->viewport_id = ds::renderer::createViewport(1280, 720, 1600, 900);
	ds::renderer::setViewportPosition(_context->viewport_id, v2(800, 450));

	m_AddBS = ds::renderer::createBlendState("alpha_blend_state", ds::BL_ONE, ds::BL_ONE, true);
	
	_rt1 = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 0));
	_rt2 = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 0));
	//snakes = new Snake(&m_Context);

	ds::Descriptor light_desc;
	light_desc.shader = ds::renderer::loadShader("lightning", "LTTech");
	assert(light_desc.shader != 0);
	light_desc.texture = 0;
	_light_desc = ds::renderer::addDescriptor(light_desc);
	_border_color = ds::Color(192, 128, 0, 255);
}


GameRenderer::~GameRenderer() {
}

// -------------------------------------------------------
// draw border
// -------------------------------------------------------
void GameRenderer::drawBorder() {
	// background
	ds::sprites::draw(v2(480, 306), ds::math::buildTexture(0, 512, 480, 306), 0.0f, 2.0f, 2.0f);
	ds::sprites::draw(v2(1280, 306), ds::math::buildTexture(0, 512, 320, 306), 0.0f, 2.0f, 2.0f);
	ds::sprites::draw(v2(480, 756), ds::math::buildTexture(0, 512, 480, 144), 0.0f, 2.0f, 2.0f);
	ds::sprites::draw(v2(1280, 756), ds::math::buildTexture(0, 512, 320, 144), 0.0f, 2.0f, 2.0f);
	// 4 corners
	ds::sprites::draw(v2(40, 860), ds::math::buildTexture(840, 0, 40, 60), 0.0f, 1.0f, 1.0f, _border_color);
	ds::sprites::draw(v2(40, 40), ds::math::buildTexture(940, 0, 40, 60), 0.0f, 1.0f, 1.0f, _border_color);
	ds::sprites::draw(v2(1560, 860), ds::math::buildTexture(840, 280, 40, 60), 0.0f, 1.0f, 1.0f, _border_color);
	ds::sprites::draw(v2(1560, 40), ds::math::buildTexture(940, 280, 40, 60), 0.0f, 1.0f, 1.0f, _border_color);
	// left and right wall
	for (int i = 0; i < 9; ++i) {
		ds::sprites::draw(v2(40, 110 + i * 80), ds::math::buildTexture(880, 0, 40, 80), 0.0f, 1.0f, 1.0f, _border_color);
		ds::sprites::draw(v2(1560, 110 + i * 80), ds::math::buildTexture(880, 280, 40, 80), 0.0f, 1.0f, 1.0f, _border_color);
	}
	// bottom and top wall
	for (int i = 0; i < 7; ++i) {
		ds::sprites::draw(v2(160 + i * 200, 870), ds::math::buildTexture(840, 40, 200, 40), 0.0f, 1.0f, 1.0f, _border_color);
		ds::sprites::draw(v2(160 + i * 200, 30), ds::math::buildTexture(960, 40, 200, 40), 0.0f, 1.0f, 1.0f, _border_color);
	}
	// missing left and right pieces
	ds::sprites::draw(v2(40, 810), ds::math::buildTexture(880, 0, 40, 40), 0.0f, 1.0f, 1.0f, _border_color);
	ds::sprites::draw(v2(1560, 810), ds::math::buildTexture(880, 280, 40, 40), 0.0f, 1.0f, 1.0f, _border_color);
	// missing top and bottom pieces
	ds::sprites::draw(v2(1505, 870), ds::math::buildTexture(840, 40, 90, 40), 0.0f, 1.0f, 1.0f, _border_color);
	ds::sprites::draw(v2(1505, 30), ds::math::buildTexture(960, 40, 90, 40), 0.0f, 1.0f, 1.0f, _border_color);
}

void GameRenderer::renderWorld() {
	PR_START("GameRenderer:render");
	ds::renderer::selectViewport(_context->viewport_id);
	drawBorder();
	_context->world->renderSingleLayer(MESSAGE_LAYER);
	_context->particles->render();
	_context->world->renderSingleLayer(OBJECT_LAYER);
	/*
	ds::renderer::setRenderTarget(_rt1);
	
	drawBorder();

	ds::sprites::flush();
	int current = ds::sprites::getDescriptorID();
	ds::sprites::setDescriptorID(_light_desc);
	_context->world->renderSingleLayer(LIGHT_LAYER);
	ds::sprites::flush();

	ds::sprites::setDescriptorID(current);
	_context->world->renderSingleLayer(MESSAGE_LAYER);
	_context->particles->render();
	_context->world->renderSingleLayer(OBJECT_LAYER);
	ds::renderer::restoreBackBuffer();
	ds::renderer::draw_render_target(_rt1);
	*/
	ds::renderer::selectViewport(0);
	PR_END("GameRenderer:render");
}
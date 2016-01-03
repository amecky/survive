#include "GameRenderer.h"
#include "Constants.h"

GameRenderer::GameRenderer(GameContext* context) : _context(context) {
	m_AddBS = ds::renderer::createBlendState("alpha_blend_state", ds::BL_ONE, ds::BL_ONE, true);
	
	_rt1 = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 0));
	_rt2 = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 0));
	//snakes = new Snake(&m_Context);

	ds::Descriptor light_desc;
	light_desc.shader = ds::renderer::loadShader("lightning", "LTTech");
	assert(light_desc.shader != 0);
	light_desc.texture = 0;
	_light_desc = ds::renderer::addDescriptor(light_desc);
}


GameRenderer::~GameRenderer() {
}


void GameRenderer::renderWorld() {
	PR_START("GameRenderer:render");
	ds::renderer::setRenderTarget(_rt1);
	//_context->world->renderSingleLayer(BG_LAYER);
	ds::sprites::draw(v2(480, 270), ds::math::buildTexture(0, 512, 480, 270), 0.0f, 2.0f, 2.0f);
	ds::sprites::draw(v2(480, 810), ds::math::buildTexture(0, 512, 480, 270), 0.0f, 2.0f, 2.0f);
	ds::sprites::draw(v2(1440, 270), ds::math::buildTexture(0, 512, 480, 270), 0.0f, 2.0f, 2.0f);
	ds::sprites::draw(v2(1440, 810), ds::math::buildTexture(0, 512, 480, 270), 0.0f, 2.0f, 2.0f);

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
	PR_END("GameRenderer:render");
}
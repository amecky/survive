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
}


GameRenderer::~GameRenderer() {
}


void GameRenderer::renderWorld() {
	ZoneTracker z("GameRenderer:render");
	//ds::renderer::selectViewport(_context->viewport_id);
	//_context->world->renderSingleLayer(EFFECT_LAYER);
	//ds::renderer::draw_screen_quad(_shader);
	//ds::renderer::setRenderTarget(_rt1);	
	_context->world->renderSingleLayer(BG_LAYER);
	_context->world->renderSingleLayer(BORDER_LAYER);
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
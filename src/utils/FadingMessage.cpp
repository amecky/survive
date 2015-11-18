#include "FadingMessage.h"
#include "..\Constants.h"

namespace messages {

	void fadingMessage(GameContext* context, const char* templateName, float ttl) {
		//ds::Sprite sprite;
		//ds::renderer::getSpriteTemplate(templateName, &sprite);
		// create light behind get ready message
		//int dim_x = sprite.texture.rect.width() + sprite.texture.rect.width() * 0.5f;
		//int dim_y = sprite.texture.rect.height() + sprite.texture.rect.height() * 0.5f;
		//ds::SID grid = context->world->create(Vector2f(512, 384), ds::math::buildTexture(600, 512, dim_x, dim_y), -1, LIGHT_LAYER);
		//context->world->setColor(grid, ds::Color(192, 0, 0, 255));
		//context->world->removeAfter(grid, ttl);
		//context->world->fadeAlphaTo(grid, 1.0f, 0.0f, ttl);
		// create get ready message
		ds::SID gid = context->world->create(Vector2f(512, 384), templateName, MESSAGE_LAYER);
		context->world->removeAfter(gid, ttl);
		context->world->fadeAlphaTo(gid, 1.0f, 0.0f, ttl);
	}

	void fadingMessage(GameContext* context, const ds::Texture& texture) {
		// create light behind get ready message
		ds::SID grid = context->world->create(Vector2f(512, 384), ds::math::buildTexture(600, 512, 440, 102), -1, LIGHT_LAYER);
		context->world->setColor(grid, ds::Color(192, 0, 0, 255));
		context->world->removeAfter(grid, context->settings->warmUpTime);
		context->world->fadeAlphaTo(grid, 1.0f, 0.0f, context->settings->warmUpTime);
		// create get ready message
		ds::SID gid = context->world->create(Vector2f(512, 384), texture, MESSAGE_LAYER);
		context->world->removeAfter(gid, context->settings->warmUpTime);
		context->world->fadeAlphaTo(gid, 1.0f, 0.0f, context->settings->warmUpTime);
	}

}
#pragma once
#include <sprites\SpriteBatch.h>
#include "GameContext.h"

namespace messages {

	void fadingMessage(GameContext* context,const ds::Texture & texture);

	void fadingMessage(GameContext* context, const char* templateName);
};


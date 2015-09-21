#pragma once
#include <sprites\SpriteBatch.h>
#include "GameContext.h"

class IntroText {

struct Message {

	ds::Sprite sprites[32];
	int num;

};

public:
	IntroText(GameContext* context);
	~IntroText();
private:
	GameContext* ctx;
};


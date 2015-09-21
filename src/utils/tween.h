#pragma once
#include <math\tweening.h>
#include <vector>

enum TweenMode {
	TM_ONCE,
	TM_INFINITY,
	TM_RESET
};

struct TweenVarFloat {
	int id;
	float* value;
	float timer;
	TweenMode mode;
	tweening::TweeningType type;
};

class tween {

typedef std::vector<TweenVarFloat> FloatVars;

public:
	tween();
	~tween();
	int add(float* v, tweening::TweeningType type, TweenMode mode = TM_ONCE);
	void tick(float dt);
private:
	FloatVars _floatVars;
};


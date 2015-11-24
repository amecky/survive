#pragma once
#include "..\utils\GameContext.h"
#include <sprites\Sprite.h>

enum EnemyType {
	ET_DODGERS,
	ET_SNAKE,
	ET_BOUNCER,
	ET_EOL
};

struct Enemy {

	ds::SID sid;
	ds::SID lightID;
	float angle;
	float wobble;
	float timer;
	v2 velocity;
	v2 position;

	Enemy() : sid(ds::INVALID_SID), lightID(ds::INVALID_SID), angle(0.0f), wobble(0.0f), timer(0.0f), velocity(0, 0), position(0, 0) {}

};

// FIXME: convert to fixed size array
typedef std::vector<Enemy> EnemyList;

class Enemies {

public:
	Enemies(GameContext* context) : _context(context) {}

	virtual ~Enemies() {}

	virtual void activate(int maxEnemies) = 0;
	
	virtual void deactivate() = 0;
	
	virtual void tick(float dt) = 0;
	
	const bool contains(ds::SID sid) const;
	
	void remove(ds::SID sid);

	virtual const EnemyType getType() const = 0;

	virtual void handleEvents(const ds::ActionEventBuffer& buffer) = 0;

	virtual bool handleImpact(ds::SID sid) = 0;

protected:
	EnemyList _list;
	GameContext* _context;
};


#pragma once
#include "..\utils\GameContext.h"


enum EnemyType {
	ET_DODGERS,
	ET_SNAKE,
	ET_EOL
};

struct EnemyDefinition {
	EnemyType type;
	int count;
	int health;
	float delay;
};

class Enemies {

public:
	Enemies(GameContext* context) : _context(context) {}

	virtual ~Enemies() {}

	virtual void activate() = 0;
	
	virtual void deactivate() = 0;
	
	virtual void tick(float dt) = 0;
	
	virtual const bool contains(ds::SID sid) const = 0;
	
	virtual void remove(ds::SID sid) = 0;

	virtual const EnemyType getType() const = 0;

	virtual int getKillCounter() const = 0;

	virtual void handleEvents(const ds::ActionEventBuffer& buffer) = 0;

	virtual void handleImpact(ds::SID sid) = 0;

protected:
	GameContext* _context;
};


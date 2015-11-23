#pragma once
#include "..\utils\GameContext.h"


enum EnemyType {
	ET_DODGERS,
	ET_SNAKE,
	ET_EOL
};





class Enemies {

public:
	Enemies(GameContext* context) : _context(context) {}

	virtual ~Enemies() {}

	virtual void activate(int maxEnemies) = 0;
	
	virtual void deactivate() = 0;
	
	virtual void tick(float dt) = 0;
	
	virtual const bool contains(ds::SID sid) const = 0;
	
	virtual void remove(ds::SID sid) = 0;

	virtual const EnemyType getType() const = 0;

	virtual void handleEvents(const ds::ActionEventBuffer& buffer) = 0;

	virtual bool handleImpact(ds::SID sid) = 0;

protected:
	GameContext* _context;
};


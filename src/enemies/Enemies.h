#pragma once
#include "..\utils\GameContext.h"
#include <sprites\Sprite.h>
#include "EnemySpawner.h"

struct StartPoint {
	v2 position;
	float timer;
	v2 normal;
};

typedef std::vector<StartPoint> StartPoints;

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
	Enemies(GameContext* context, const SpawnerData& data);

	virtual ~Enemies();

	void activate(int maxEnemies);
	
	virtual void move(float dt) = 0;

	void tick(float dt);
	
	const bool contains(ds::SID sid) const;
	
	void remove(ds::SID sid);

	virtual const EnemyType getType() const = 0;

	virtual void handleEvents(const ds::ActionEventBuffer& buffer) = 0;

	virtual bool handleImpact(ds::SID sid) = 0;

	virtual void killAll() = 0;

	virtual void create(const StartPoint& startPoint) = 0;

protected:
	void createSpawner(const SpawnerData& data);
	EnemyList _list;
	GameContext* _context;
	EnemySpawner* _spawner;
	StartPoints _startPoints;
	float _spawnTimer;
	SpawnerData _spawnData;
	int _maxEnemies;
	int _counter;
};


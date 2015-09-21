#pragma once
#include "utils\GameContext.h"

enum ShootingMode {
	SM_SHOOTING,
	SM_IDLE
};

class Player {

public:
	Player(GameContext* context);
	~Player();
	void create();
	void move(float dt);
	void kill();
	void shootBullets(float dt);	
	void setShooting(ShootingMode mode);
private:
	void fireBullet();
	void fireBullet(const Vector2f& pos, const Vector2f& direction);
	GameContext* _context;
	ds::SID _id;
	float _angle;
	Vector2f _position;
	ds::SID _lightIndex;
	ShootingMode _shootingMode;
	float _shootTimer;
};


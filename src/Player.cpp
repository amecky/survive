#include "Player.h"
#include "Constants.h"

Player::Player(GameContext* context) : _context(context) {
	_shootingMode = SM_IDLE;
	_shootTimer = 0.0f;
}


Player::~Player() {
}

// --------------------------------------------------------------------------
// create player
// --------------------------------------------------------------------------
void Player::create() {
	_position = Vector2f(800, 450);
	_id = _context->world->create(_position, "player", OBJECT_LAYER);
	//_id = _context->world->create(_position, ds::math::buildTexture(40, 0, 40, 40), 0.0f, 1.0f, 1.0f, ds::Color(237, 195, 154, 255), OT_PLAYER, OBJECT_LAYER);
	//_id = _context->world->create(_position, ds::math::buildTexture(40, 0, 40, 40),OT_PLAYER, OBJECT_LAYER);
	_context->playerID = _id;
	_context->world->attachCollider(_id, Vector2f(46.0f, 46.0f), OT_PLAYER,OBJECT_LAYER);
	_angle = 0.0f;
	//_context->trails->add(_id, 5.0f, 3);
	//_lightIndex = _context->world->create(_position, "lightning", LIGHT_LAYER);// _context->lights->add_permanent(_position, ds::Color(0, 255, 0, 255));
	//_context->world->setColor(_lightIndex, ds::Color(0, 255, 0, 255));
	_shootingMode = SM_IDLE;
	_shootTimer = _context->fireRate;
	//ds::renderer::setViewportPosition(_context->viewport_id, _position);
	_alive = true;
}

// --------------------------------------------------------------------------
// move player
// --------------------------------------------------------------------------
void Player::move(float dt) {
	if (_alive) {
		//Vector2f pp = _context->world->getPosition(_id);
		Vector2f v;
		if (GetAsyncKeyState('W') & 0x8000) {
			v += Vector2f(0, 1);
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			v += Vector2f(0, -1);
		}
		if (GetAsyncKeyState('A') & 0x8000) {
			v += Vector2f(-1, 0);
		}
		if (GetAsyncKeyState('D') & 0x8000) {
			v += Vector2f(1, 0);
		}		

		v2 cursor_pos = ds::renderer::getMousePosition();
		//const ds::Viewport& vw = ds::renderer::getViewport(_context->viewport_id);
		//v2 wp = vw.convertToWorld(cursor_pos);
		v2 wp = cursor_pos;
		_position += v * dt * _context->playerSpeed;
		Vector2f diff = wp - _position;
		_angle = ds::vector::calculateRotation(diff);
		_context->world->setRotation(_id, _angle);
		ds::vector::clamp(_position, v2(60, 60), v2(1540, 840));
		_context->world->setPosition(_id, _position);
		//_position = rwp;
		_context->playerPos = _position;
		_context->world_pos = _position;
		//LOG << "wp: " << DBG_V2(wp) << " world_pos: " << DBG_V2(rwp);
		//ds::renderer::setViewportPosition(_context->viewport_id, _position);
	}
}

void Player::setShooting(ShootingMode mode) {
	_shootingMode = mode;
}

void Player::kill() {
	if (_alive) {
		_context->particles->start(PLAYER_EXPLOSION, _position);
		//_context->particles->start(8, _position);
		_context->world->remove(_id);
		//_context->world->remove(_lightIndex);
		_context->playerID = ds::INVALID_SID;
		_shootingMode = SM_IDLE;
		_alive = false;
	}
}

// --------------------------------------------------------------------------
// shoot bullets
// --------------------------------------------------------------------------
void Player::shootBullets(float dt) {
	if (_shootingMode == SM_SHOOTING) {
		_shootTimer -= dt;
		if (_shootTimer <= 0.0f) {
			fireBullet();
			_shootTimer = _context->settings->playerFireRate;
		}
	}
}
// --------------------------------------------------------------------------
// fire bullet
// --------------------------------------------------------------------------
void Player::fireBullet() {
	if (_alive) {
		if (_context->doubleFire) {
			Vector2f p = _position;
			ds::vector::addRadial(p, 24.0f, _angle);
			//p.y += 7.0f;
			//p.x += 20.0f;
			fireBullet(p);
			//p.y -= 14.0f;
			//fireBullet(p, V2_RIGHT);
		}
		else {
			Vector2f p = _position;
			ds::vector::addRadial(p, 24.0f, _angle);
			fireBullet(p);
		}
		if (_context->tripleShot) {
			Vector2f p = _position;
			p.y += 30.0f;
			fireBullet(p);
			p.y -= 60.0f;
			fireBullet(p);
			p.y += 30.0f;
			p.x -= 20.0f;
			fireBullet(p);
		}
	}
}

// --------------------------------------------------------------------------
// fire bullet at specific position and direction
// --------------------------------------------------------------------------
void Player::fireBullet(const Vector2f& pos) {
	Vector2f v = ds::vector::getRadialVelocity(_angle, _context->settings->bulletVelocity);
	ds::SID sid = _context->world->create(pos, "bullet", OBJECT_LAYER);
	_context->world->moveBy(sid, v);
	_context->world->setRotation(sid, _angle);
	_context->world->attachCollider(sid, Vector2f(10.0f, 10.0f), OT_BULLET,OBJECT_LAYER);
	//_context->trails->add(sid, 4.0f, BULLET_TRAIL);
}
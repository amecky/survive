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
	_position = Vector2f(512, 384);
	_id = _context->world->create(_position, "player",OBJECT_LAYER);
	_context->playerID = _id;
	_context->world->attachCollider(_id, Vector2f(46.0f, 46.0f), OT_PLAYER,OBJECT_LAYER);
	_angle = 0.0f;
	_position = Vector2f(100, 384);
	_context->trails->add(_id, 5.0f, 3);
	//_lightIndex = _context->world->create(_position, "lightning", LIGHT_LAYER);// _context->lights->add_permanent(_position, ds::Color(0, 255, 0, 255));
	//_context->world->setColor(_lightIndex, ds::Color(0, 255, 0, 255));
	_shootingMode = SM_IDLE;
	_shootTimer = _context->fireRate;
	_alive = true;
}

// --------------------------------------------------------------------------
// move player
// --------------------------------------------------------------------------
void Player::move(float dt) {
	if (_alive) {
		Vector2f pp = _context->world->getPosition(_id);
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
		v2 wp;
		float dx = _context->world_pos.x - 1280.0f / 2.0f;
		if (dx < 0.0f) {
			dx = 0.0f;
		}
		if (dx > 320.0f) {
			dx = 320.0f;
		}
		wp.x = cursor_pos.x + dx;

		float dy = _context->world_pos.y - 720.0f / 2.0f;
		if (dy < 0.0f) {
			dy = 0.0f;
		}
		if (dy > 180.0f) {
			dy = 180.0f;
		}
		wp.y = cursor_pos.y + dy;


		Vector2f& mp = ds::renderer::getMousePosition();
		Vector2f diff = wp - pp;
		_angle = ds::vector::calculateRotation(diff);
		_context->world->setRotation(_id, _angle);

		ds::Viewport& vp = ds::renderer::getSelectedViewport();
		pp += v * dt * _context->playerSpeed;

		ds::vector::clamp(pp, v2(60, 60), v2(1540, 840));

		_context->world->setPosition(_id, pp);
		_position = pp;
		_context->playerPos = pp;
		//_context->world->setPosition(_lightIndex, _position);
		_context->world_pos = pp;
		ds::renderer::setViewportPosition(_context->viewport_id, pp);
	}
}

void Player::setShooting(ShootingMode mode) {
	_shootingMode = mode;
}

void Player::kill() {
	if (_alive) {
		_context->particles->start(6, _position);
		_context->particles->start(8, _position);
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
			fireBullet(p, V2_RIGHT);
			//p.y -= 14.0f;
			//fireBullet(p, V2_RIGHT);
		}
		else {
			Vector2f p = _position;
			ds::vector::addRadial(p, 24.0f, _angle);
			//p.x += 20.0f;
			fireBullet(p, V2_RIGHT);
		}
		if (_context->tripleShot) {
			Vector2f p = _position;
			p.y += 30.0f;
			fireBullet(p, V2_UP);
			p.y -= 60.0f;
			fireBullet(p, V2_DOWN);
			p.y += 30.0f;
			p.x -= 20.0f;
			fireBullet(p, V2_LEFT);
		}
	}
}

// --------------------------------------------------------------------------
// fire bullet at specific position and direction
// --------------------------------------------------------------------------
void Player::fireBullet(const Vector2f& pos, const Vector2f& direction) {
	//float angle = _angle + ds::math::random(DEGTORAD(-10.0f), DEGTORAD(10.0f));
	Vector2f v = ds::vector::getRadialVelocity(_angle, _context->settings->bulletVelocity);
	ds::SID sid = _context->world->create(pos, "bullet", OBJECT_LAYER);
	//m_World.scaleTo(sid, Vector2f(0.1f, 1.0f), Vector2f(1.0f, 1.0f), 0.4f);
	_context->world->moveBy(sid, v);
	_context->world->setRotation(sid, _angle);

	//ds::SID lid = _context->world->create(pos, "bullet_light", LIGHT_LAYER);// _context->lights->add_permanent(_position, ds::Color(0, 255, 0, 255));
	//_context->world->setColor(lid, ds::Color(255, 253, 190, 255));
	//_context->world->moveBy(lid, v);
	//_context->world->scale(lid, 0.5f, 0.5f);

	_context->world->attachCollider(sid, Vector2f(10.0f, 10.0f), OT_BULLET,OBJECT_LAYER);
	//_context->trails->addFrameBased(sid, angle, 1, 4);
}
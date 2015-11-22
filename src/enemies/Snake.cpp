#include "Snake.h"
#include "..\Constants.h"
#include <math\GameMath.h>
#include "..\utils\util.h"

// ------------------------------------------------
// Snake
// ------------------------------------------------
Snake::Snake(GameContext* context) : Enemies(context) {
	m_DotTex = ds::math::buildTexture(145, 0, 10, 10);
	m_Active = false;
}

// ------------------------------------------------
// destructor
// ------------------------------------------------
Snake::~Snake() {
}

// ------------------------------------------------
// create tail
// ------------------------------------------------
void Snake::createTail(const Vector2f& start,float scale) {
	ds::SID tailID = _context->world->create(start, "snake_tail");
	_context->world->setColor(tailID, _context->settings->snakeTailColor);
	_context->world->scale(tailID, scale, scale);
	_context->world->attachCollider(tailID, SNAKE_TAIL, OBJECT_LAYER);
	SnakeTrail st;
	st.id = tailID;
	st.distance = 2.4f * (scale);
	m_Trails.push_back(st);
}

void Snake::deactivate() {
	// FIXME: kill all
}
// ------------------------------------------------
// start
// ------------------------------------------------
void Snake::activate() {
	Vector2f start(200, 200);
	float angle = PI;
	m_HeadID = _context->world->create(start, "snake_head");
	m_ShieldID = _context->world->create(start, "snake_shield");
	_context->world->setColor(m_HeadID, ds::Color(192, 0, 0, 255));
	_context->world->attachCollider(m_HeadID, SNAKE_HEAD, OBJECT_LAYER);
	_context->world->attachCollider(m_ShieldID, SNAKE_SHIELD, OBJECT_LAYER);
	for (int i = 0; i < MAX_SNAKE_TAILS; ++i) {
		ds::vector::addRadial(start, 32.0f, PI);
		float scale = 1.0f - static_cast<float>(i) / static_cast<float>(MAX_SNAKE_TAILS) * 0.2f;
		createTail(start, scale);
	}
	m_Sector = 1;
	m_Sector = util::buildSingleCurve(Vector2f(200, 200), m_Sector, &m_Path,220.0f, false);
	m_Path.build();
	_context->world->followPath(m_HeadID, &m_Path, _context->settings->snakeVelocity);
	m_Active = true;
	m_Kills = 0;
	m_KillSize = 10;
	m_SpawnTimer = 0.0f;
	m_ShieldActive = true;
	m_HeadShots = 0;
}

// ------------------------------------------------
// move trail
// ------------------------------------------------
void Snake::tick(float dt) {
	ds::SID id = m_HeadID;
	float angle = 0.0f;
	TrailList::iterator it = m_Trails.begin();
	while (it != m_Trails.end()) {
		Vector2f prev = _context->world->getPosition(id);
		Vector2f current = _context->world->getPosition(it->id);
		ds::math::followRelative(prev, current, &angle, it->distance, dt * _context->settings->snakeFollowVelocity);
		_context->world->setPosition(it->id, current);
		_context->world->setRotation(it->id, ds::math::reflect(angle));
		id = it->id;
		++it;
	}
	// move shield
	if (m_ShieldActive) {
		_context->world->setPosition(m_ShieldID, _context->world->getPosition(m_HeadID));
	}

	// spawn new tail if necessary
	m_SpawnTimer += dt;
	if (m_SpawnTimer > _context->settings->spawnTimer) {
		m_SpawnTimer = 0.0f;
		Vector2f start;
		if (m_Trails.size() > 0) {
			SnakeTrail& last = m_Trails.back();
			start = _context->world->getPosition(last.id);
		}
		else {
			start = _context->world->getPosition(m_HeadID);
		}
		ds::vector::addRadial(start, 32.0f, PI);
		float scale = 0.8f;
		createTail(start, scale);
	}
}

// ------------------------------------------------
// debug - draw path
// ------------------------------------------------
void Snake::debug() {
	if (m_Active) {
		util::drawCurve(m_Path, m_DotTex);
	}
}

// ------------------------------------------------
// handle events
// ------------------------------------------------
void Snake::handleEvents(const ds::ActionEventBuffer& buffer) {
	if (buffer.num > 0) {
		for (int i = 0; i < buffer.num; ++i) {
			if (buffer.events[i].type == ds::AT_FOLLOW_PATH && buffer.events[i].sid == m_HeadID) {
				//LOG << "head done";
				Vector2f end = m_Path.getElement(0).p3;
				m_Sector = util::buildSingleCurve(end, m_Sector, &m_Path,220.0f, false);
				m_Path.build();
				_context->world->followPath(m_HeadID, &m_Path, _context->settings->snakeVelocity);
			}
		}
	}
}

// ------------------------------------------------
// contains tail
// ------------------------------------------------
bool Snake::containsTail(ds::SID sid) {
	TrailList::iterator it = m_Trails.begin();
	while (it != m_Trails.end()) {
		if (it->id == sid) {
			return true;
		}
		++it;
	}
	return false;
}

// ------------------------------------------------
// remove tail
// ------------------------------------------------
void Snake::removeTail(ds::SID sid) {
	TrailList::iterator it = m_Trails.begin();
	while (it != m_Trails.end()) {
		if (it->id == sid) {
			_context->world->remove(sid);
			it = m_Trails.erase(it);
			++m_Kills;
			if (m_ShieldActive && (m_Kills > m_KillSize)) {
				//ctx->world->setColor(m_HeadID, ds::Color(0, 192, 0, 255));
				_context->world->remove(m_ShieldID);
				m_ShieldActive = false;
			}
		}
		else {
			++it;
		}
	}
}


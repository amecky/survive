#include "Snake.h"
#include "..\Constants.h"
#include <math\GameMath.h>
#include "..\utils\util.h"

// ------------------------------------------------
// Snake
// ------------------------------------------------
Snake::Snake(GameContext* context,const SpawnerData& data) : Enemies(context,data) {
	m_Active = false;
	m_HeadID = ds::INVALID_SID;
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
	ds::SID tailID = _context->world->create(start, "snake_tail", OBJECT_LAYER);
	_context->world->scale(tailID, scale, scale);
	_context->world->attachCollider(tailID, SNAKE_TAIL, OBJECT_LAYER);
	SnakeTrail st;
	st.id = tailID;
	st.distance = 2.4f * (scale);
	m_Trails.push_back(st);
}

// ------------------------------------------------
// start
// ------------------------------------------------
void Snake::create(const StartPoint& start) {
	if (!m_Active){
		float angle = PI;
		m_HeadID = _context->world->create(start.position, "snake_head", OBJECT_LAYER);
		_context->world->setColor(m_HeadID, ds::Color(192, 0, 0, 255));
		_context->world->attachCollider(m_HeadID, SNAKE_HEAD, OBJECT_LAYER);
		v2 sp = start.position;
		for (int i = 0; i < MAX_SNAKE_TAILS; ++i) {
			ds::vector::addRadial(sp, 32.0f + i * 32.0f, PI);
			float scale = 1.0f - static_cast<float>(i) / static_cast<float>(MAX_SNAKE_TAILS)* 0.2f;
			createTail(sp, scale);
		}
		_context->world->followTarget(m_HeadID, _context->playerID, 100.0f);
		m_Active = true;
	}
}

// ------------------------------------------------
// move trail
// ------------------------------------------------
void Snake::move(float dt) {
	if (m_Active) {
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
	}
}

// ------------------------------------------------
// handle events
// ------------------------------------------------
void Snake::handleEvents(const ds::ActionEventBuffer& buffer) {
	/*
	if (buffer.num > 0) {
		for (int i = 0; i < buffer.num; ++i) {
			if (buffer.events[i].type == ds::AT_FOLLOW_PATH && buffer.events[i].sid == m_HeadID) {
				//LOG << "head done";
				Vector2f end = m_Path.getElement(0).p3;
				const SpawnPoint& next = _spawner->random();
				util::buildSingleCurve(end, next.position, &m_Path,220.0f, false);
				m_Path.build();
				_context->world->followPath(m_HeadID, &m_Path, 4.0f);
			}
		}
	}
	*/
}


// ------------------------------------------------
// remove tail
// ------------------------------------------------
/*
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
*/

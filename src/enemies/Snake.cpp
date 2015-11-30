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
	_trailCount = 0;
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
		_sector = ds::math::random(0, 11);
		v2 ssp = util::buildStartingPoint(_sector,v2(40,40));
		float angle = PI;
		m_HeadID = _context->world->create(ssp, "snake_head", OBJECT_LAYER);
		_context->world->setColor(m_HeadID, ds::Color(192, 0, 0, 255));
		_context->world->attachCollider(m_HeadID, SNAKE_HEAD, OBJECT_LAYER);
		v2 sp = ssp;
		for (int i = 0; i < 1; ++i) {
			ds::vector::addRadial(sp, 32.0f + i * 32.0f, PI);
			float scale = 1.0f - static_cast<float>(i) / static_cast<float>(MAX_SNAKE_TAILS)* 0.2f;
			createTail(sp, scale);
		}
		_sector = util::buildSingleCurve(ssp, _sector, &_path, 220.0f, false);
		_path.build();
		//_context->world->followTarget(m_HeadID, _context->playerID, 100.0f);
		_context->world->followPath(m_HeadID, &_path, 4.0f);
		m_Active = true;
		_previousHeadPos = ssp;
	}
}

// ------------------------------------------------
// move trail
// ------------------------------------------------
void Snake::move(float dt) {
	// http://genericgamedev.com/general/snake-smooth-and-accurate-following-behaviour/
	if (m_Active) {
		if ((_trailCount + 1) >= 256) {
			_trailCount = 0;
		}
		ds::SID id = m_HeadID;
		v2 current = _context->world->getPosition(id);
		if (sqr_length(current - _previousHeadPos) > 400.0f) {
			_trailPath[_trailCount++] = current;
			_previousHeadPos = current;
		}
		v2 tp = _context->world->getPosition(m_Trails[0].id);
		for (int i = 0; i < _trailCount; ++i) {
			v2 diff = tp - _trailPath[i];
			if (sqr_length(diff) > 400.0f) {
				float percentageAlongSegment = 20.0f / length(diff);
				tp = _trailPath[i] + diff * percentageAlongSegment;
				_context->world->setPosition(m_Trails[0].id, tp);
				break;
			}
		}
		/*
		
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
		*/
	}
	
}

void Snake::render_debug() {
	for (int i = 0; i < _trailCount; ++i) {
		ds::sprites::draw(_trailPath[i], ds::math::buildTexture(477,42,16,16));
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
				v2 end = _path.getElement(0).p3;
				//const SpawnPoint& next = _spawner->random();
				_sector = util::buildSingleCurve(end, _sector, &_path,220.0f, false);
				_path.build();
				_context->world->followPath(m_HeadID, &_path, 4.0f);
			}
		}
	}
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

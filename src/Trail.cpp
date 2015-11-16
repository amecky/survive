#include "Trail.h"
#include <particles\ParticleSystem.h>
#include "utils\GameContext.h"

Trail::Trail(GameContext* context) : ctx(context) {
}


Trail::~Trail(void) {
}

// --------------------------------------------------------------------------
// add new trail
// --------------------------------------------------------------------------
void Trail::add(ds::SID sid,float distance,int particleSystem) {
	TrailPiece tp;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.distance = distance;
	tp.type = 0;
	tp.emitType = TET_DISTANCE;
	tp.frameCounter = 0;
	m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// add new trail with specific rotation
// --------------------------------------------------------------------------
void Trail::add(ds::SID sid,float angle,float distance,int particleSystem) {
	TrailPiece tp;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.distance = distance;
	tp.type = 1;
	tp.angle = angle;
	tp.emitType = TET_DISTANCE;
	tp.frameCounter = 0;
	m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// add frame based
// --------------------------------------------------------------------------
void Trail::addFrameBased(ds::SID sid, float angle, int frames, int particleSystem) {
	TrailPiece tp;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.frames = frames;
	tp.type = 1;
	tp.angle = angle;
	tp.emitType = TET_FRAMES;
	tp.frameCounter = 0;
	m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// add time based
// --------------------------------------------------------------------------
void Trail::addTimeBased(ds::SID sid, float angle, float ttl, int particleSystem) {
	TrailPiece tp;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.time = ttl;
	tp.type = 1;
	tp.angle = angle;
	tp.emitType = TET_TIME;
	tp.timer = 0.0f;
	m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// removes trail
// --------------------------------------------------------------------------
void Trail::remove(ds::SID sid) {
	if (m_Pieces.find(sid) != m_Pieces.end()) {
		m_Pieces.erase(sid);
	}
}

// --------------------------------------------------------------------------
// tick
// --------------------------------------------------------------------------
void Trail::tick(float dt) {
	PR_START("Trail:tick")
	Pieces::iterator it = m_Pieces.begin();
	while (it != m_Pieces.end()) {
		if (ctx->world->contains(it->first)) {
			Vector2f p = ctx->world->getPosition(it->first);
			if (it->second.emitType == TET_DISTANCE) {				
				if (distance(p, it->second.prevPosition) > it->second.distance) {
					ds::ParticleGeneratorData data(it->second.prevPosition);
					data.rotation = it->second.angle;
					ctx->particles->start(it->second.particleSystem, data);
					it->second.prevPosition = p;
				}
			}
			else if (it->second.emitType == TET_FRAMES) {
				++it->second.frameCounter;
				if (it->second.frameCounter >= it->second.frames) {
					it->second.frameCounter = 0;
					ds::ParticleGeneratorData data(it->second.prevPosition);
					data.rotation = it->second.angle;
					ctx->particles->start(it->second.particleSystem, data);
					it->second.prevPosition = p;
				}
			}
			else if (it->second.emitType == TET_TIME) {
				it->second.timer += dt;
				if (it->second.timer >= it->second.time) {
					it->second.timer = 0.0f;
					ds::ParticleGeneratorData data(it->second.prevPosition);
					data.rotation = it->second.angle;
					ctx->particles->start(it->second.particleSystem, data);
					it->second.prevPosition = p;
				}
			}
			++it;
		}
		else {
			it = m_Pieces.erase(it);
		}		
	}
	PR_END("Trail:tick")
}
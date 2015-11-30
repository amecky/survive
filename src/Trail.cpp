#include "Trail.h"
#include <particles\ParticleSystem.h>
#include "utils\GameContext.h"

Trail::Trail(GameContext* context,int maxTrails) : ctx(context) , _maxTrails(maxTrails) , _currentTrails(0) {
	_pieces = new TrailPiece[maxTrails];
}


Trail::~Trail(void) {
	delete[] _pieces;
}

// --------------------------------------------------------------------------
// add new trail
// --------------------------------------------------------------------------
void Trail::add(ds::SID sid,float distance,int particleSystem) {
	TrailPiece& tp = _pieces[_currentTrails++];
	tp.id = sid;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.distance = distance;
	tp.type = 0;
	tp.emitType = TET_DISTANCE;
	tp.frameCounter = 0;
	//m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// add new trail with specific rotation
// --------------------------------------------------------------------------
void Trail::add(ds::SID sid,float angle,float distance,int particleSystem) {
	TrailPiece& tp = _pieces[_currentTrails++];
	tp.id = sid;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.distance = distance;
	tp.type = 1;
	tp.angle = angle;
	tp.emitType = TET_DISTANCE;
	tp.frameCounter = 0;
	//m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// add frame based
// --------------------------------------------------------------------------
void Trail::addFrameBased(ds::SID sid, float angle, int frames, int particleSystem) {
	TrailPiece& tp = _pieces[_currentTrails++];
	tp.id = sid;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.frames = frames;
	tp.type = 1;
	tp.angle = angle;
	tp.emitType = TET_FRAMES;
	tp.frameCounter = 0;
	//m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// add time based
// --------------------------------------------------------------------------
void Trail::addTimeBased(ds::SID sid, float angle, float ttl, int particleSystem) {
	TrailPiece& tp = _pieces[_currentTrails++];
	tp.id = sid;
	tp.particleSystem = particleSystem;
	tp.prevPosition = ctx->world->getPosition(sid);
	tp.time = ttl;
	tp.type = 1;
	tp.angle = angle;
	tp.emitType = TET_TIME;
	tp.timer = 0.0f;
	//m_Pieces[sid] = tp;
}

// --------------------------------------------------------------------------
// removes trail
// --------------------------------------------------------------------------
void Trail::remove(ds::SID sid) {
	for (int i = 0; i < _currentTrails; ++i) {
		if (_pieces[i].id == sid)  {
			kill(i);
			break;
		}
	}
}

void Trail::kill(int index) {
	if (_currentTrails > 0) {
		TrailPiece& p = _pieces[index];
		p = _pieces[--_currentTrails];
	}
}
// --------------------------------------------------------------------------
// tick
// --------------------------------------------------------------------------
void Trail::tick(float dt) {
	PR_START("Trail:tick");
	for (int i = 0; i < _currentTrails; ++i) {
		TrailPiece& piece = _pieces[i];
		if (ctx->world->contains(piece.id)) {
			v2 p = ctx->world->getPosition(piece.id);
			if (piece.emitType == TET_DISTANCE) {
				if (sqr_distance(p, piece.prevPosition) > (piece.distance * piece.distance)) {
					ds::ParticleGeneratorData data(piece.prevPosition);
					data.rotation = piece.angle;
					ctx->particles->start(piece.particleSystem, data);
					piece.prevPosition = p;
				}
			}
			else if (piece.emitType == TET_FRAMES) {
				++piece.frameCounter;
				if (piece.frameCounter >= piece.frames) {
					piece.frameCounter = 0;
					ds::ParticleGeneratorData data(piece.prevPosition);
					data.rotation = piece.angle;
					ctx->particles->start(piece.particleSystem, data);
					piece.prevPosition = p;
				}
			}
			else if (piece.emitType == TET_TIME) {
				piece.timer += dt;
				if (piece.timer >= piece.time) {
					piece.timer = 0.0f;
					ds::ParticleGeneratorData data(piece.prevPosition);
					data.rotation = piece.angle;
					ctx->particles->start(piece.particleSystem, data);
					piece.prevPosition = p;
				}
			}
		}
		else {
			kill(i);
		}		
	}
	PR_END("Trail:tick");
}
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
void Trail::add(ds::SID sid,float distance,int particleSystem,float offset) {
	TrailPiece& tp = _pieces[_currentTrails++];
	prepare(tp, sid, 0.0f, particleSystem, TET_DISTANCE);
	tp.distance = distance;
	tp.distanceOffset = offset;
}

// --------------------------------------------------------------------------
// add new trail with specific rotation
// --------------------------------------------------------------------------
void Trail::add(ds::SID sid,float angle,float distance,int particleSystem, float offset) {
	TrailPiece& tp = _pieces[_currentTrails++];
	prepare(tp, sid, angle, particleSystem, TET_DISTANCE);
	tp.distance = distance;
	tp.distanceOffset = offset;
}

// --------------------------------------------------------------------------
// add frame based
// --------------------------------------------------------------------------
void Trail::addFrameBased(ds::SID sid, float angle, int frames, int particleSystem) {
	TrailPiece& tp = _pieces[_currentTrails++];
	prepare(tp, sid, angle, particleSystem, TET_FRAMES);
	tp.frames = frames;
}

// --------------------------------------------------------------------------
// add time based
// --------------------------------------------------------------------------
void Trail::addTimeBased(ds::SID sid, float angle, float ttl, int particleSystem) {
	TrailPiece& tp = _pieces[_currentTrails++];
	prepare(tp, sid, angle, particleSystem, TET_TIME);
	tp.time = ttl;
}

// --------------------------------------------------------------------------
// prepare common settings
// --------------------------------------------------------------------------
void Trail::prepare(TrailPiece& piece, ds::SID sid, float angle, int particleSystem, TrailEmitType type) {
	piece.id = sid;
	piece.particleSystem = particleSystem;
	piece.prevPosition = ctx->world->getPosition(sid);
	piece.type = 1;
	piece.angle = angle;
	piece.emitType = type;
	piece.timer = 0.0f;
	piece.emitted = 0;
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
	ZoneTracker z("Trail:tick");
	for (int i = 0; i < _currentTrails; ++i) {
		TrailPiece& piece = _pieces[i];
		if (ctx->world->contains(piece.id)) {
			v2 p = ctx->world->getPosition(piece.id);
			if (piece.emitType == TET_DISTANCE) {
				float angle = ctx->world->getRotation(piece.id);
				float d = piece.distance * piece.distance;
				if (sqr_distance(p, piece.prevPosition) > d) {
					if (piece.distanceOffset > 0.0f) {
						v2 np = piece.prevPosition;
						ds::vector::addRadial(np, piece.distanceOffset, angle + PI);
						ctx->particles->start(piece.particleSystem, np);
					}
					else {
						ctx->particles->start(piece.particleSystem, piece.prevPosition);
					}
					piece.prevPosition = p;
				}
				++piece.emitted;
			}
			else if (piece.emitType == TET_FRAMES) {
				++piece.frameCounter;
				if (piece.frameCounter >= piece.frames) {
					piece.frameCounter = 0;
					ctx->particles->start(piece.particleSystem, piece.prevPosition);
					piece.prevPosition = p;
				}
			}
			else if (piece.emitType == TET_TIME) {
				piece.timer += dt;
				if (piece.timer >= piece.time) {
					piece.timer = 0.0f;
					ctx->particles->start(piece.particleSystem, piece.prevPosition);
					piece.prevPosition = p;
				}
			}
		}
		else {
			kill(i);
		}		
	}
}
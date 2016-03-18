#include "Cubes.h"
#include "..\Constants.h"
#include <utils\Log.h>
#include <io\FileRepository.h>

const int ALL_TYPES[] = { OT_FOLLOWER, OT_BIG_CUBE, OT_HUGE_CUBE, OT_MEGA_CUBE, OT_SUPER_CUBE };

// ---------------------------------------
// load cube definitions
// ---------------------------------------
bool CubeDefinitions::loadData(const ds::JSONReader& reader) {
	_definitions.clear();
	int cats[32];
	int num = reader.get_categories(cats, 32);
	for (int i = 0; i < num; ++i) {
		CubeDefinition def;
		reader.get_int(cats[i], "type", &def.type);
		sprintf_s(def.name, 20, "%s", reader.get_string(cats[i],"name"));
		reader.get_float(cats[i], "velocity", &def.velocity);
		reader.get_float(cats[i], "velocity_variance", &def.velocityVariance);
		reader.get_int(cats[i], "num_stars", &def.numStars);
		reader.get_int(cats[i], "energy", &def.energy);
		reader.get_int(cats[i], "next_type", &def.nextType);
		reader.get_float(cats[i], "grow_ttl", &def.growTTL);
		reader.get_int(cats[i], "trail_system", &def.trailSystem);
		reader.get_float(cats[i], "trail_distance", &def.trailDistance);
		_definitions.push_back(def);
	}
	return true;
}

// ---------------------------------------
// load wave definitions
// ---------------------------------------
bool WaveDefinitions::loadData(const ds::JSONReader& reader) {
	_definitions.clear();
	int cats[32];
	int num = reader.get_categories(cats, 32);
	for (int i = 0; i < num; ++i) {
		WaveDefinition def;
		reader.get_int(cats[i], "cube_type", &def.cubeType);
		reader.get_int(cats[i], "max_concurrent", &def.maxConcurrent);
		reader.get_int(cats[i], "num_spawn", &def.numSpawn);
		reader.get_float(cats[i], "spawn_ttl", &def.spawnTTL);
		_definitions.push_back(def);
	}
	return true;
}

// ---------------------------------------
// Cubes
// ---------------------------------------
Cubes::Cubes(GameContext* context) : _context(context) , _world(context->world) {
	//_cubeDefintions.load();
	ds::repository::load(&_cubeDefintions);
	//_waveDefinitions.load();
	ds::repository::load(&_waveDefinitions);
	//_cubeEmitterSettings.load();
	ds::repository::load(&_cubeEmitterSettings);

	for (size_t i = 0; i < _waveDefinitions.size(); ++i) {
		WaveRuntime rt;
		rt.current = 0;
		rt.definitionIndex = i;
		rt.timer = 0.0f;
		rt.total = 0;
		_waveRuntimes.push_back(rt);
	}

}

Cubes::~Cubes() {
}

// ---------------------------------------
// create ball
// ---------------------------------------
void Cubes::createBall(const v2& pos, int current, int total, int waveDefinitionIndex) {
	v2 position = pos;
	const WaveDefinition& waveDefinition = _waveDefinitions.get(waveDefinitionIndex);
	const CubeDefinition cubeDefinition = _cubeDefintions.get(waveDefinition.cubeType);
	if (total > 1) {
		float ra = static_cast<float>(current) / static_cast<float>(total)* TWO_PI;
		float rd = ds::math::random(40.0f, 60.0f);
		v2 pp = v2(rd * cos(ra), rd * sin(ra));
		position += pp;
	}
	ds::SID sid = _world->create(position, cubeDefinition.name,OBJECT_LAYER);
	_world->attachCollider(sid);
	Ball* data = (Ball*)_world->attach_data(sid, sizeof(Ball),OT_FOLLOWER);
	assert(data != 0);
	float angle = ds::math::random(0.0f, TWO_PI);
	data->velocity = ds::vector::getRadialVelocity(angle, ds::math::random(cubeDefinition.velocity - cubeDefinition.velocityVariance, cubeDefinition.velocity + cubeDefinition.velocityVariance));
	_world->startBehavior(sid, "wiggle_scale");
	data->force = v2(0, 0);
	data->def_index = waveDefinition.cubeType;
	data->wave_index = waveDefinitionIndex;
	data->energy = cubeDefinition.energy;
}

// ---------------------------------------
// handle events
// ---------------------------------------
void Cubes::handleEvents(const ds::ActionEventBuffer& buffer) {
	for (int i = 0; i < buffer.events.size(); ++i) {
		const ds::ActionEvent& event = buffer.events[i];
		if (_world->contains(event.sid)) {
			int t = _world->getType(event.sid);
			if (t == OT_BIG_CUBE || t == OT_HUGE_CUBE || t == OT_FOLLOWER || t == OT_MEGA_CUBE || t == OT_SUPER_CUBE || t == OT_FOLLOWER) {
				if (event.type == ds::AT_SCALE_BY_PATH) {
					Ball* data = (Ball*)_world->get_data(event.sid);
					XASSERT(data != 0, "No Ball data found for %d", event.sid);
					_world->moveBy(event.sid, data->velocity, true);
					//_world->rotate(event.sid, 180.0f, 0.8f, -1);
					//_world->moveBy(data->lightIndex, data->velocity, true);
				}
				if (event.type == ds::AT_SCALE) {
					Ball* data = (Ball*)_world->get_data(event.sid);
					XASSERT(data != 0, "No Ball data found for %d", event.sid);
					_world->moveBy(event.sid, data->velocity, true);
					//_world->rotate(event.sid, 180.0f, 0.8f, -1);
					//_world->moveBy(data->lightIndex, data->velocity, true);
				}
			}
		}
	}
}

// ---------------------------------------
// seek
// ---------------------------------------
void Cubes::seek(const v2& target, float velocity) {
	ds::SID sids[256];
	int num = _world->find_by_type(OT_FOLLOWER, sids, 256);
	for (int i = 0; i < num; ++i) {
		Ball* data = (Ball*)_world->get_data(sids[i]);
		XASSERT(data != 0, "No Ball data found for %d", sids[i]);
		v2 diff = target - _world->getPosition(sids[i]);
		v2 n = normalize(diff);
		v2 desired = n * velocity;
		data->force += desired;
	}
}

// ------------------------------------------------
// separate
// ------------------------------------------------
void Cubes::separate(const v2& target, float minDistance, float relaxation) {
	float sqrDist = minDistance * minDistance;
	ds::SID sids[256];
	int num = _world->find_by_type(OT_FOLLOWER, sids, 256);
	for (int i = 0; i < num; ++i) {
		Ball* data = (Ball*)_world->get_data(sids[i]);
		XASSERT(data != 0, "No Ball data found for %d", sids[i]);
		int cnt = 0;
		v2 separationForce = v2(0, 0);
		v2 averageDirection = v2(0, 0);
		v2 distance = v2(0, 0);
		v2 currentPos = _world->getPosition(sids[i]);
		for (int j = 0; j < num; j++) {
			if (i != j) {
				v2 dist = _world->getPosition(sids[j]) - currentPos;
				if (sqr_length(dist) < sqrDist) {
					++cnt;
					separationForce += dist;
					separationForce = normalize(separationForce);
					separationForce = separationForce * relaxation;
					averageDirection += separationForce;
				}
			}
		}
		if (cnt > 0) {
			data->force -= averageDirection;
		}
	}
}

// ------------------------------------------------
// align
// ------------------------------------------------
void Cubes::align(const v2& target, float desiredDistance) {
	float sqrDesired = desiredDistance * desiredDistance;
	ds::SID sids[256];
	int num = _world->find_by_type(OT_FOLLOWER, sids, 256);
	for (int i = 0; i < num; ++i) {
		Ball* data = (Ball*)_world->get_data(sids[i]);
		XASSERT(data != 0, "No Ball data found for %d", sids[i]);
		int cnt = 0;
		v2 separationForce = v2(0, 0);
		v2 averageDirection = v2(0, 0);
		v2 distance = v2(0, 0);
		v2 currentPos = _world->getPosition(sids[i]);
		for (int j = 0; j < num; j++) {
			if (i != j) {
				v2 dist = _world->getPosition(sids[j]) - currentPos;
				if (sqr_length(dist) < sqrDesired) {
					++cnt;
					averageDirection += data->velocity;
				}
			}
		}
		if (cnt > 0) {
			averageDirection /= static_cast<float>(cnt);
			data->force += averageDirection;
		}
	}
}
// ---------------------------------------
// move balls
// ---------------------------------------
void Cubes::move(float dt) {
	int count = 0;
	ds::SID sids[256];
	int num = _world->find_by_type(OT_FOLLOWER, sids, 256);
	for (int i = 0; i < num; ++i) {
		Ball* data = (Ball*)_world->get_data(sids[i]);
		XASSERT(data != 0, "No Ball data found for %d", sids[i]);
		data->force = v2(0, 0);
	}
	seek(_context->world_pos, 120.0f);
	separate(_context->world_pos, 40.0f, 15.0f);
	align(_context->world_pos, 40.0f);
	for (int i = 0; i < num; ++i) {
		Ball* data = (Ball*)_world->get_data(sids[i]);
		XASSERT(data != 0, "No Ball data found for %d", sids[i]);
		v2 p = _world->getPosition(sids[i]);
		p += data->force * dt;
		_world->setPosition(sids[i],p);
		v2 diff = _context->world_pos - p;
		v2 n = normalize(diff);
		_world->setRotation(sids[i], ds::vector::calculateRotation(n));
	}
}

// ---------------------------------------
// kill balls in range
// ---------------------------------------
int Cubes::killBalls(const v2& bombPos, KilledBall* killedBalls) {
	int count = 0;
	ds::SID sids[256];
	int num = _world->find_by_types(ALL_TYPES, 4, sids, 256);
	for (int i = 0; i < num; ++i) {
		v2 p = _world->getPosition(sids[i]);
		Ball* data = (Ball*)_world->get_data(sids[i]);
		XASSERT(data != 0, "No Ball data found for %d", sids[i]);
		// FIXME!!!
		float size = 20.0f;
		if (ds::math::checkCircleIntersection(bombPos, BOMB_EXPLOSION_RADIUS, p, size)) {
			WaveRuntime& runtime = _waveRuntimes[data->wave_index];
			--runtime.current;
			KilledBall& kb = killedBalls[count++];
			kb.position = p;
			kb.type = _world->getType(sids[i]);
			_world->remove(sids[i]);
		}
	}
	return count;
}

// ---------------------------------------
// kill cube
// ---------------------------------------
int Cubes::kill(ds::SID sid) {
	int ret = -1;
	if (_world->contains(sid)) {
		int type = _world->getType(sid);
		if (type == OT_FOLLOWER || type == OT_BIG_CUBE || type == OT_HUGE_CUBE || type == OT_MEGA_CUBE || type == OT_SUPER_CUBE) {
			Ball* data = (Ball*)_world->get_data(sid);
			--data->energy;
			if (data->energy <= 0) {
				ret = type;
				v2 p = _world->getPosition(sid);
				_context->particles->start(BALL_EXPLOSION, p);
				_world->remove(sid);
				const CubeDefinition& def = _cubeDefintions.get(data->def_index);				
				if (def.nextType != -1) {
					// FIXME: create splitCube method and take current velocity and make new one perpendicular
					for (int i = 0; i < 2; ++i) {
						createBall(p, i, 2, def.nextType);
					}
				}
			}
		}
	}
	return ret;
}
// ---------------------------------------
// kill all
// ---------------------------------------
void Cubes::killAll(bool explode) {
	ds::SID sids[256];
	int num = _world->find_by_types(ALL_TYPES, 4, sids, 256);
	for ( int i = 0; i < num; ++i) {
		if (explode) {
			v2 p = _world->getPosition(sids[i]);
			_context->particles->start(BALL_EXPLOSION, p);
		}
		_world->remove(sids[i]);
	}
}

void Cubes::emitt(int type, const v2& pos, const v2& normal) {
	createBall(pos, 0, 0, type);
}
// ------------------------------------------------
// emitt
// ------------------------------------------------
void Cubes::emitt(int type) {
	/*
	const SpawnPoint& spawn = _emitter->random();
	*/
	/*	
	const WaveDefinition& waveDef = _waveDefinitions.get(type);
	for (int i = 0; i < waveDef.numSpawn; ++i) {
		createBall(spawn.position, i, waveDef.numSpawn, type);
	}
	*/
	/*
	ds::SID sid = _world->create(spawn.position, "CubeEmitter", OBJECT_LAYER);
	CubeEmitter* emitter = (CubeEmitter*)_world->attach_data(sid, sizeof(CubeEmitter));
	emitter->timer = 0.0;
	emitter->cube_type = type;
	// FIXME: take values from settings
	emitter->spawnTTL = _cubeEmitterSettings.spawnTTL;
	emitter->ttl = _cubeEmitterSettings.ttl;
	emitter->dead = false;
	_world->scaleByPath(sid, &_cubeEmitterSettings.scalePath, _cubeEmitterSettings.scaleTTL);
	_world->flashColor(sid, _cubeEmitterSettings.startFlash, _cubeEmitterSettings.endFlash, _cubeEmitterSettings.flashAmplitude, -1);
	*/
}

// ------------------------------------------------
// tick and create new cubes
// ------------------------------------------------
void Cubes::spawn(float dt) {
	/*
	for (size_t i = 0; i < _waveDefinitions.size(); ++i) {
		const WaveDefinition& waveDef = _waveDefinitions.get(i);
		WaveRuntime& runtime = _waveRuntimes[i];
		runtime.timer += dt;
		if (runtime.timer >= waveDef.spawnTTL) {
			runtime.timer = 0.0f;
			int delta = waveDef.maxConcurrent - runtime.current;
			if (delta > 0) {
				const SpawnPoint& spawn = _emitter->random();				
				for (int j = 0; j < waveDef.numSpawn; ++j) {
					createBall(spawn.position, j, waveDef.numSpawn, i);
					++runtime.total;
					++runtime.current;
				}
			}
		}

	}
	*/
	ds::SID emitters[128];
	int num = _world->find_by_type(OT_CUBE_EMITTER, emitters, 128);
	for (int i = 0; i < num; ++i) {
		CubeEmitter* data = (CubeEmitter*)_world->get_data(emitters[i]);
		if (data != 0) {
			data->timer += dt;
			if (data->timer > data->spawnTTL && !data->dead) {
				createBall(_world->getPosition(emitters[i]), 1, 1, data->cube_type);
				data->dead = true;
			}
			if ( data->timer > data->ttl ) {
				_context->particles->start(EMITTER_EXPLOSION, _world->getPosition(emitters[i]));
				_world->remove(emitters[i]);
			}
		}
	}
}

// ------------------------------------------------
// activate
// ------------------------------------------------
void Cubes::activate() {
	_emitted = 0;
	// FIXME:
	killAll(false);
	//_emitter->rebuild();
	//_spawner_position = v2(200, 200);
	for (size_t i = 0; i < _waveRuntimes.size(); ++i) {
		WaveRuntime& rt = _waveRuntimes[i];
		rt.current = 0;
		rt.timer = 0.0f;
		rt.total = 0;
	}
}

// ------------------------------------------------
// reload
// ------------------------------------------------
void Cubes::reload() {
	_cubeEmitterSettings.load();
}

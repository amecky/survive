#include "Cubes.h"

bool CubeDefinitions::loadData(JSONReader& reader) {
	const ds::Array<Category*>& all = reader.getCategories();
	for (size_t i = 0; i < all.size(); ++i) {
		Category* c = all[i];
		if (c->getName() == "cube") {
			CubeDefinition def;
			def.type = c->getInt("type", 0);
			const char* t = c->getProperty("name");
			sprintf_s(def.name, 20, "%s", t);
			def.velocity = c->getFloat("velocity",0.0f);
			def.velocityVariance = c->getFloat("velocity_variance", 0.0f);
			def.numStars = c->getInt("num_stars", 1);
			def.behaviorBits = c->getInt("behavior_bits", 1);
			def.growTTL = c->getFloat("grow_ttl", 0.2f);
			def.energy = c->getInt("energy", 1);
			def.nextType = c->getInt("next_type", -1);
			_definitions.push_back(def);
		}
	}
	return true;
}

bool CubeDefinitions::loadData(ds::SimpleJSONReader& reader) {
	int cats[32];
	int num = reader.get_categories(cats, 32);
	for (int i = 0; i < num; ++i) {
		CubeDefinition def;
		reader.get_int(cats[i],"type", &def.type);
		const char* t = reader.get_string(cats[i], "name");
		sprintf_s(def.name, 20, "%s", t);
		reader.get_float(cats[i], "velocity", &def.velocity);
		reader.get_float(cats[i], "velocity_variance", &def.velocityVariance);
		reader.get_int(cats[i], "num_stars", &def.numStars);
		reader.get_int(cats[i], "behavior_bits", &def.behaviorBits);
		reader.get_float(cats[i], "grow_ttl", &def.growTTL);
		reader.get_int(cats[i], "energy", &def.energy);
		reader.get_int(cats[i], "next_type", &def.nextType);
		_definitions.push_back(def);
	}
	return true;
}

bool WaveDefinitions::loadData(JSONReader& reader) {
	const ds::Array<Category*>& all = reader.getCategories();
	for (size_t i = 0; i < all.size(); ++i) {
		Category* c = all[i];
		if (c->getName() == "wave") {
			WaveDefinition def;
			def.cubeType = c->getInt("cube_type", 0);
			def.maxConcurrent = c->getInt("max_concurrent", 10);
			def.numSpawn = c->getInt("num_spawn", 1);
			def.spawnTTL = c->getFloat("spawn_ttl", 1.0f);
			_definitions.push_back(def);
		}
	}
	return true;
}

bool WaveDefinitions::loadData(ds::SimpleJSONReader& reader) {
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

Cubes::Cubes()
{
}


Cubes::~Cubes()
{
}

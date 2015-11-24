#include "Enemies.h"

// ------------------------------------------------
// check if this is a enemy
// ------------------------------------------------
const bool Enemies::contains(ds::SID sid) const {
	for (int i = 0; i < _list.size(); ++i) {
		if (_list[i].sid == sid) {
			return true;
		}
	}
	return false;
}

// ------------------------------------------------
// remove enemy
// ------------------------------------------------
void Enemies::remove(ds::SID sid) {
	EnemyList::iterator it = _list.begin();
	while (it != _list.end()) {
		if (it->sid == sid) {
			_context->world->remove(sid);
			_context->trails->remove(sid);
			_context->world->remove(it->lightID);
			it = _list.erase(it);
		}
		else {
			++it;
		}
	}
}
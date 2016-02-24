#include "util.h"
#include <math\GameMath.h>
#include <utils\Log.h>
#include "..\Constants.h"
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>

namespace util {

	Vector2f convert(int gridX, int gridY)  {
		float halfGridSize = GRID_SIZE * 0.5f;
		float x = halfGridSize + gridX * GRID_SIZE;
		float y = 84.0f + halfGridSize + gridY * GRID_SIZE;
		return Vector2f(x, y);
	}

	Vector2f convert(const Vector2i& gridPos) {
		return convert(gridPos.x, gridPos.y);
	}

	// -------------------------------------------
	// 12
	// 03
	// -------------------------------------------
	v2 buildStartingPoint(int sector) {
		v2 p;
		
		switch ( sector ) {
			case 0: p = ds::math::random(v2(  40,  40), v2(  80,  80)); break;
			case 1: p = ds::math::random(v2(  40, 640), v2(  80, 680)); break;
			case 2: p = ds::math::random(v2(1120, 640), v2(1160, 680));	break;
			case 3: p = ds::math::random(v2(1120,  40), v2(1160,  80)); break;
		}
		return p;
	}

	v2 buildStartingPoint(int sector,const v2& border) {
		v2 p;
		float dx = (ds::renderer::getScreenWidth() - 2.0f * border.x) / 3.0f;
		float dy = (ds::renderer::getScreenHeight() - 2.0f * border.y) / 3.0f;
		float max_x = ds::renderer::getScreenWidth() - border.x;
		float max_y = ds::renderer::getScreenHeight() - border.y;
		float x = 0.0f;
		float y = 0.0f;
		switch (sector) {
			case 0: {
				p.x = border.x; 
				p.y = border.y + ds::math::random(0.0f, dy);
				break;
			}
			case 1: {
				p.x = border.x;
				p.y = border.y + ds::math::random(0.0f, dy) + dy;
				break;
			}
			case 2: {
				p.x = border.x;
				p.y = border.y + ds::math::random(0.0f, dy) + dy * 2.0f;
				break;
			}
			case 3: {
				p.x = border.x + ds::math::random(0.0f,dx);
				p.y = max_y;
				break;
			}
			case 4: {
				p.x = border.x + ds::math::random(0.0f, dx) + dx;
				p.y = max_y;
				break;
			}
			case 5: {
				p.x = border.x + ds::math::random(0.0f, dx) + dx * 2.0f;
				p.y = max_y;
				break;
			}
			case 6: {
				p.x = max_x;
				p.y = border.y + ds::math::random(0.0f, dy);
				break;
			}
			case 7: {
				p.x = max_x;
				p.y = border.y + ds::math::random(0.0f, dy) + dy;
				break;
			}
			case 8: {
				p.x = max_x;
				p.y = border.y + ds::math::random(0.0f, dy) + dy * 2.0f;
				break;
			}
			case 9: {
				p.x = border.x + ds::math::random(0.0f, dx);
				p.y = border.y;
				break;
			}
			case 10: {
				p.x = border.x + ds::math::random(0.0f, dx) + dx;
				p.y = border.y;
				break;
			}
			case 11: {
				p.x = border.x + ds::math::random(0.0f, dx) + dx * 2.0f;
				p.y = border.y;
				break;
			}
			
		}
		return p;
	}

	int pickRandomSector(int current) {
		static int others[3];
		int num = 0;
		for ( int i = 0; i < 4; ++i ) {
			if ( i != current ) {
				others[num++] = i;
			}
		}
		return others[ds::math::random(0,2)];
	}

	void simpleMove(Vector2f& p,float* angle,float t) {
		*angle = PI;
		p.x = 1300.0f - 1300.0f * t;
		p.y = 612.0f;
		if ( t >= 0.3f && t <= 0.6f ) {
			p.y = 612.0f - 200.0f * (t - 0.3f)/0.3f;
			*angle = DEGTORAD(225.0f);
		}
		else if ( t > 0.6f ) {
			p.y = 412.0f;
		}
		//p.y = 384 + sin(t * TWO_PI * 4.f) * 100.0f * sin(t * TWO_PI);
		
	}

	void waveMove(Vector2f& p,float* angle,float t) {
		p.x = 1300.0f - 1300.0f * t;
		p.y = 512 + sin(t * TWO_PI * 4.f) * 100.0f * sin(t * TWO_PI);
		*angle = 0.0f;
	}

	void circularSineMove(Vector2f& p,float* angle,float t) {
		p.y = (15.0f * cos(t * -4.0f * TWO_PI)) + (640.0f + (180.0f * sin(t * 2.0f * TWO_PI)));
		p.x = (15.0f * sin(t * -4.0f * TWO_PI)) + (512.0f + (200.0f * cos(t / 2.0f * TWO_PI)));
		*angle = 0.0f;
	}

	void buildPath(ds::CubicBezierPath* path) {
		int sector = ds::math::random(0, 3);
		Vector2f target = buildStartingPoint(sector);
		int next = buildSingleCurve(target, sector,path,false);
		for (int i = 0; i < 2; ++i) {
			const ds::BezierCurve& c = path->getElement(i);
			next = buildSingleCurve(c.p3, next,path,true);
		}
		path->build();
		for (int i = 0; i < path->size(); ++i) {
			const ds::BezierCurve& c = path->getElement(i);
			//LOG << i << " p0: " << DBG_V2(c.p0) << " p1: " << DBG_V2(c.p1) << " p2: " << DBG_V2(c.p2) << " p3: " << DBG_V2(c.p3);
		}
	}

	int buildSingleCurve(const Vector2f& start, int sector, ds::CubicBezierPath* path,float amplitude, bool append) {
		int next = sector + 5;
		if (next > 11) {
			next -= 11;
		}
		v2 end = buildStartingPoint(next,v2(40,40));
		//LOG << "start: " << DBG_V2(start) << " end: " << DBG_V2(end) << " sector: " << sector << " next: " << next;
		float da = 0.25f;//ds::math::random(0.15f, 0.35f);
		float db = 0.75f;// ds::math::random(0.65f, 0.85f);
		v2 diff = end - start;
		v2 pa = start + diff * da;
		v2 pb = start + diff * db;
		v2 rpa = pa;
		ds::vector::rotate(rpa, DEGTORAD(90.0f));
		v2 nrpa = normalize(rpa);
		nrpa *= -amplitude;
		pa += nrpa;

		v2 rpb = pb;
		ds::vector::rotate(rpb, DEGTORAD(90.0f));
		v2 nrpb = normalize(rpb);
		nrpb *= amplitude;
		pb += nrpb;
		if (append) {
			path->add(pa, pb, end);
		}
		else {
			path->create(start, pa, pb, end);
		}
		return next;
	}

	int buildSingleCurve(const v2& start, const v2& end, ds::CubicBezierPath* path, float amplitude, bool append) {
		//LOG << "start: " << DBG_V2(start) << " end: " << DBG_V2(end) << " sector: " << sector << " next: " << next;
		float da = 0.25f;//ds::math::random(0.15f, 0.35f);
		float db = 0.75f;// ds::math::random(0.65f, 0.85f);
		Vector2f diff = end - start;
		Vector2f pa = start + diff * da;
		Vector2f pb = start + diff * db;
		Vector2f rpa = pa;
		ds::vector::rotate(rpa, DEGTORAD(90.0f));
		Vector2f nrpa = normalize(rpa);
		nrpa *= -amplitude;
		pa += nrpa;

		Vector2f rpb = pb;
		ds::vector::rotate(rpb, DEGTORAD(90.0f));
		Vector2f nrpb = normalize(rpb);
		nrpb *= amplitude;
		pb += nrpb;
		if (append) {
			path->add(pa, pb, end);
		}
		else {
			path->create(start, pa, pb, end);
		}
		return -1;
	}

	void drawCurve(ds::CubicBezierPath& path,const ds::Texture& dotTex) {
		Vector2f p;
		for (int i = 0; i < 64; ++i) {
			float step = static_cast<float>(i) / 64.0f;
			//path.get(step,&p);
			path.approx(step, &p);
			ds::sprites::draw(p, dotTex);
		}
		for (int j = 0; j < path.size(); ++j) {
			const ds::BezierCurve& e = path.getElement(j);
			ds::sprites::draw(e.p0, dotTex, 0.0f, 1.0f, 1.0f, ds::Color(192, 0, 0, 255));
			ds::sprites::draw(e.p1, dotTex, 0.0f, 1.0f, 1.0f, ds::Color(192, 0, 192, 255));
			ds::sprites::draw(e.p2, dotTex, 0.0f, 1.0f, 1.0f, ds::Color(192, 192, 0, 255));
			ds::sprites::draw(e.p3, dotTex, 0.0f, 1.0f, 1.0f, ds::Color(0, 0, 192, 255));
		}
	}

	v2 pickSpawnPoint(const v2& playerPos) {
		// convert player position to grid
		int px = (playerPos.x - 80.0f) / 80.0f;
		int py = (playerPos.y - 45.0f) / 90.0f;
		// the board is divided into cells 18x9 (80x90 pixel)
		int x = 0;
		int y = 0;
		bool match = true;
		while (match) {
			x = 80 + ds::math::random(0, 18) * 80;
			y = 45 + ds::math::random(0, 9) * 90;
			if (x != px || y != py) {
				match = false;
			}
		}
		return v2(x, y);
	}
}
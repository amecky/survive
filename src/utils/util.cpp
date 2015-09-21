#include "util.h"
#include <math\GameMath.h>
#include <utils\Log.h>
#include "..\Constants.h"
#include <sprites\SpriteBatch.h>

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

	Vector2f buildStartingPoint(int sector) {		
		Vector2f p;
		switch ( sector ) {
		case 0 : 
			p = Vector2f(ds::math::random(20,200),ds::math::random(20,150));
			break;
		case 1 : 
			p = Vector2f(ds::math::random(900,1000),ds::math::random(20,150));
			break;
		case 2 : 
			p = Vector2f(ds::math::random(900,1000),ds::math::random(600,700));
			break;
		case 3 : 
			p = Vector2f(ds::math::random(20,200),ds::math::random(600,700));
			break;
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
		int next = sector + 1;
		if (next > 3) {
			next = 0;
		}
		Vector2f end = buildStartingPoint(next);
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
		return next;
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
}
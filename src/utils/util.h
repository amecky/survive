#pragma once
#include <Vector.h>
#include <math\CubicBezierPath.h>
#include <renderer\render_types.h>

namespace util {

	Vector2f convert(int gridX, int gridY);

	Vector2f convert(const Vector2i& gridPos);

	Vector2f buildStartingPoint(int sector);

	v2 buildStartingPoint(int sector, const v2& border);

	int pickRandomSector(int current);

	void simpleMove(Vector2f& p,float* angle,float t);

	void waveMove(Vector2f& p,float* angle,float t);

	void circularSineMove(Vector2f& p,float* angle,float t);

	void buildPath(ds::CubicBezierPath* path);

	int buildSingleCurve(const Vector2f& start, int sector, ds::CubicBezierPath* path,float amplitude = 140.0f, bool append = true);

	int buildSingleCurve(const v2& start, const v2& end, ds::CubicBezierPath* path, float amplitude = 140.0f, bool append = true);

	void drawCurve(ds::CubicBezierPath& path, const ds::Texture& dotTex);

	v2 pickSpawnPoint(const v2& playerPos);
}
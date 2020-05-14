#pragma once

#include "Vec3.h"

class Ray
{
public:
	Vec3 dir;
	Point3 origin;

	Ray(const Vec3& dir_v, const Point3& origin_v): dir(dir_v), origin(origin_v)
	{
		dir = dir / dir.length();
	}

	Point3 pointAt(const float s) const { return origin + dir * s; }
};

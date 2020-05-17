#pragma once

#include "Vec3.h"

class Ray
{
public:
	Vec3 dir;
	Point3 origin;

	Ray(const Vec3& dir_v, const Point3& origin_v);

	Point3 pointAt(const float s) const;
};

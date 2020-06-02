#pragma once

#include "Vec3.h"

class Ray
{
private:
	Point3 orig;
	Vec3 dir;

public:
	Ray();
	Ray(const Point3& orig_, const Vec3& dir_);

	Point3 origin() const;
	Vec3 direction() const;

	Point3 at(const double t) const;
};

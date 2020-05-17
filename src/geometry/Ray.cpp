#include "Ray.h"

Ray::Ray(const Vec3& dir_v, const Point3& origin_v): dir(dir_v), origin(origin_v)
{
	dir = dir / dir.length();
}

Point3 Ray::pointAt(const float s) const
{
	return origin + dir * s;
}

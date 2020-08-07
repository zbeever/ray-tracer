#include "Ray.h"

Ray::Ray()
{ }

Ray::Ray(const Point3& orig_, const Vec3& dir_, const double time_): orig(orig_), dir(dir_), tm(time_)
{ }

Point3 Ray::origin() const
{
	return orig;
}

Vec3 Ray::direction() const
{
	return dir;
}

double Ray::time() const
{
	return tm;
}

Point3 Ray::at(const double t) const
{
	return orig + t * dir;
}

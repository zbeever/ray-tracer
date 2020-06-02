#include "Ray.h"

Ray::Ray()
{ }

Ray::Ray(const Point3& orig_, const Vec3& dir_): orig(orig_), dir(dir_)
{ }

Point3 Ray::origin() const
{
	return orig;
}

Vec3 Ray::direction() const
{
	return dir;
}

Point3 Ray::at(const double t) const
{
	return orig + t * dir;
}

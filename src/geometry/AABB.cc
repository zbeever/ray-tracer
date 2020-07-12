#include "AABB.h"

AABB::AABB()
{ }

AABB::AABB(const Point3& a, const Point3 b): _min(a), _max(b)
{ }

Point3 AABB::min() const
{
	return _min;
}

Point3 AABB::max() const
{
	return _max;
}


bool AABB::hit(const Ray& ray, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const
{
	for (int a = 0; a < 3; ++a)
	{
		auto invD = 1.0f / ray.direction()[a];
		auto t0 = (min()[a] - ray.origin()[a]) * invD;
		auto t1 = (max()[a] - ray.origin()[a]) * invD;

		if (invD < 0.0f)
		{
			std::swap(t0, t1);
		}

		double tmin = t0 > t_min ? t0 : t_min;
		double tmax = t1 < t_max ? t1 : t_max;

		if (tmax <= tmin)
		{
			return false;
		}
	}
	return true;
}

AABB surrounding_box(AABB box0, AABB box1)
{
	Point3 small(std::min(box0.min().x(), box1.min().x()),
		     std::min(box0.min().y(), box1.min().y()),
		     std::min(box0.min().z(), box1.min().z()));

	Point3 large(std::min(box0.max().x(), box1.max().x()),
		     std::min(box0.max().y(), box1.max().y()),
		     std::min(box0.max().z(), box1.max().z()));

	return AABB(small, large);
}

#pragma once

#include "Record.h"

class AABB
{
public:
	Point3 _min, _max;

	AABB();
	AABB(const Point3& a, const Point3& b);

	Point3 min() const;
	Point3 max() const;
	bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const;	
	static std::shared_ptr<AABB> make(const Point3& a, const Point3& b);
};

AABB surrounding_box(AABB box0, AABB box1);

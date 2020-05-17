#pragma once

#include <array>
#include "Surface.h"

bool triIntersect(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max, const std::array<Point3, 3>& vertex);

class Triangle: public Surface
{
public:
	std::array<Point3, 3> vertex;

	Triangle(const Point3& p0_v, const Point3& p1_v, const Point3& p2_v);

	void shift(const Vec3& offset);
	void scale(const float k, const Point3& o);

	bool hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const;
};

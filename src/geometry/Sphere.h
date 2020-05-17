#pragma once

#include "Surface.h"

class Sphere: public Surface
{
public:
	Point3 origin;
	float radius;

	Sphere(Point3 origin_v, float radius_v);

	void shift(const Vec3& offset);
	void scale(const float k, const Point3& o);

	bool hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const;
};

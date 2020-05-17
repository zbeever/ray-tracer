#pragma once

#include "Surface.h"

class Plane: public Surface
{
public:
	Point3 pos;
	Vec3 normal;

	Plane(Point3 pos_v, Vec3 normal_v);

	void shift(const Vec3& offset);
	void scale(const float k, const Point3& o);

	bool hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const;
};

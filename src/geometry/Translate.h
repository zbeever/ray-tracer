#pragma once

#include "Surface.h"

class Translate: public Surface
{
public:
	std::shared_ptr<Surface> ptr;
	Vec3 offset;

	Translate(std::shared_ptr<Surface> p, const Vec3& displacement);

	virtual bool hit(const Ray& r, double t_min, double t_max, Record& rec) const;
	virtual bool bounding_box(double t0, double t1, AABB& output_box) const;
};

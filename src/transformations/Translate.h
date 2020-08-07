#pragma once

#include "../engine/Surface.h"

class Translate: public Surface
{
public:
	std::shared_ptr<Surface> ptr;
	Vec3 offset;

	Translate(std::shared_ptr<Surface> p, const Vec3& displacement);

	virtual bool hit(const Ray& r, double t_min, double t_max, Record& rec, std::mt19937& rgen) const;
	virtual bool bounding_box(double t0, double t1, AABB& output_box) const;
	static std::shared_ptr<Translate> applyTo(std::shared_ptr<Surface> p, const Vec3& displacement);
};

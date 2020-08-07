#pragma once

#include "../engine/Surface.h"

class MovingSphere: public Surface
{
public:
	Point3 center0, center1;
	double time0, time1;
	double radius;
	std::shared_ptr<Material> mat_ptr;

	MovingSphere();
	MovingSphere(const Point3& cen0, const Point3& cen1, const double t0, const double t1, const double radius_, std::shared_ptr<Material> mat_ptr_);

	Point3 center(const double time) const;
	virtual bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const;
	virtual bool bounding_box(const double t0, const double t1, AABB& output_box) const;
};

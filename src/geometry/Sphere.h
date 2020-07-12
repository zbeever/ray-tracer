#pragma once

#include "Surface.h"

class Sphere: public Surface
{
public:
	Point3 center;
	double radius;
	std::shared_ptr<Material> mat_ptr;

	Sphere();
	Sphere(const Point3& center_, const double radius_, std::shared_ptr<Material> mat_ptr_);

	virtual bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const;	
	virtual bool bounding_box(const double t0, const double t1, AABB& output_box) const;
};

void get_sphere_uv(const Vec3& p, double& u, double& v);

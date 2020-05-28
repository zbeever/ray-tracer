#pragma once

#include "Surface.h"

class Sphere: public Surface
{
public:
	Point3 center;
	double radius;
	std::shared_ptr<Material> mat_ptr;

	Sphere()
	{ }

	Sphere(const Point3& center_, const double radius_, std::shared_ptr<Material> mat_ptr_): center(center_), radius(radius_), mat_ptr(mat_ptr_)
	{ }

	virtual bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const;	
};

bool Sphere::hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const
{
	Vec3 oc = ray.origin() - center;

	auto a = ray.direction().length_squared();
	auto half_b = dot(oc, ray.direction());
	auto c = oc.length_squared() - radius*radius;
	auto discriminant = half_b*half_b - a*c;

	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;

		if (temp > t_max || temp < t_min) {
			temp = (-half_b + root) / a;
		}

		if (temp > t_max || temp < t_min) {
			return false;
		}

		rec.t = temp;
		rec.p = ray.at(rec.t);
		Vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(ray, outward_normal);
		rec.mat_ptr = mat_ptr;

		return true;
	}

	return false;
}

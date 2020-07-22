#pragma once

#include "Surface.h"
#include "ONB.h"

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

	virtual double pdf_value(const Point3& o, const Vec3& v, std::mt19937& rgen) const
	{
		Record rec;
		if (!this->hit(Ray(o, v), 1e-3, infinity, rec, rgen))
			return 0;

		auto cos_theta_max = sqrt(1 - radius * radius / (center - o).length_squared());
		auto solid_angle = 2 * pi * (1 - cos_theta_max);

		return 1 / solid_angle;
	}

	virtual Vec3 random(const Vec3& o, std::mt19937& rgen) const
	{
		Vec3 direction = center - o;
		auto distance_squared = direction.length_squared();
		ONB uvw;
		uvw.build_from_w(direction);
		return uvw.local(random_to_sphere(radius, distance_squared, rgen));
	}

	inline Vec3 random_to_sphere(double radius, double distance_squared, std::mt19937& rgen) const
	{
		auto r1 = random_double(rgen);
		auto r2 = random_double(rgen);
		auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

		auto phi = 2 * pi * r1;
		auto x = cos(phi) * sqrt(1 - z * z);
		auto y = sin(phi) * sqrt(1 - z * z);

		return Vec3(x, y, z);
	}
};

void get_sphere_uv(const Vec3& p, double& u, double& v);

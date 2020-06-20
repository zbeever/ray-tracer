#include "Sphere.h"
#include <cmath>

Sphere::Sphere()
{ }

Sphere::Sphere(const Point3& center_, const double radius_, std::shared_ptr<Material> mat_ptr_): center(center_), radius(radius_), mat_ptr(mat_ptr_)
{ }

bool Sphere::hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const
{
	Vec3 oc = ray.origin() - center;

	auto a = ray.direction().length_squared();
	auto half_b = dot(oc, ray.direction());
	auto c = oc.length_squared() - radius*radius;
	auto discriminant = half_b*half_b - a*c;

	if (discriminant > 0)
	{
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;

		if (temp > t_max || temp < t_min)
		{
			temp = (-half_b + root) / a;
		}

		if (temp > t_max || temp < t_min)
		{
			return false;
		}

		rec.t = temp;
		rec.p = ray.at(rec.t);
		Vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(ray, outward_normal);
		rec.mat_ptr = mat_ptr;
		get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);

		return true;
	}

	return false;
}

bool Sphere::bounding_box(const double t0, const double t1, AABB& output_box) const
{
	output_box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
	return true;
}

void get_sphere_uv(const Vec3& p, double& u, double& v)
{
	auto phi = atan2(p.z(), p.x());
	auto theta = asin(p.y());
	u = 1 - (phi + pi) / (2 * pi);
	v = (theta + pi / 2) / pi;
}

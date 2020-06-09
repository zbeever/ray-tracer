#include "MovingSphere.h"

MovingSphere::MovingSphere()
{ }

MovingSphere::MovingSphere(const Point3& cen0, const Point3& cen1, const double t0, const double t1, const double radius_, std::shared_ptr<Material> mat_ptr_): center0(cen0), center1(cen1), time0(t0), time1(t1), radius(radius_), mat_ptr(mat_ptr_)
{ }

Point3 MovingSphere::center(const double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const
{
	Vec3 oc = ray.origin() - center(ray.time());

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
		Vec3 outward_normal = (rec.p - center(ray.time())) / radius;
		rec.set_face_normal(ray, outward_normal);
		rec.mat_ptr = mat_ptr;

		return true;
	}

	return false;
}

bool MovingSphere::bounding_box(const double t0, const double t1, AABB& output_box) const
{
	AABB box0(center(t0) - Vec3(radius, radius, radius), center(t0) + Vec3(radius, radius, radius));
	AABB box1(center(t1) - Vec3(radius, radius, radius), center(t1) + Vec3(radius, radius, radius));
	output_box = surrounding_box(box0, box1);
	return true;
}

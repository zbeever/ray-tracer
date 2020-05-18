#include "Sphere.h"

Sphere::Sphere(Point3 origin_v, float radius_v): origin(origin_v), radius(radius_v)
{ }

void Sphere::shift(const Vec3& offset)
{
	origin += offset;
	return;
}

void Sphere::scale(const float k, const Point3& o)
{
	radius *= k;
	origin = k*(origin - o) + o;
	return;
}

bool Sphere::hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const
{
	Vec3 v = r.origin - origin;

	if (v.length() < radius)
	{
		return false;
	}

	float b = 2.0f * dot(r.dir, v);
	float c = dot(v, v) - radius*radius;
	float disc = b*b - 4*c;

	if (disc > 0)
	{
		float s = (-b + std::sqrt(disc)) * 0.5;

		if (s >= s_min && s < s_max)
		{
			Point3 pos = r.pointAt(s);
			Vec3 normal = (pos - origin) / (pos - origin).length();

			hit_record.pos = pos;
			hit_record.normal = normal;
			hit_record.s = s;
			
			return true;
		}

		s = (-b - std::sqrt(disc)) * 0.5;

		if (s >= s_min && s < s_max)
		{
			Point3 pos = r.pointAt(s);
			Vec3 normal = (pos - origin) / (pos - origin).length();

			hit_record.pos = pos;
			hit_record.normal = normal;
			hit_record.s = s;
			
			return true;
		}
	}

	return false;
}

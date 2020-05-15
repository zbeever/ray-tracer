#pragma once

#include "Surface.h"

class Sphere: public Surface
{
private:
	Point3 origin;
	float radius;

public:
	Sphere(Point3 origin_v, float radius_v): origin(origin_v), radius(radius_v)
	{ }

	void shift(const Vec3& offset)
	{
		origin += offset;

		return;
	}

	void scale(const float k)
	{
		radius *= k;

		return;
	}

	bool getIntersection(const Ray& r, PointRecord& hit_record) const
	{
		Vec3 v = r.origin - origin;

		if (v.length() < radius)
			return false;

		float b = 2.0f * dot(r.dir, v);
		float c = dot(v, v) - radius*radius;
		float disc = b*b - 4*c;

		if (disc > 0) {
			float s = (-b - std::sqrt(disc)) * 0.5;
			Point3 pos = r.pointAt(s);
			Vec3 normal = (pos - origin) / (pos - origin).length();

			hit_record.pos = pos;
			hit_record.normal = normal;
			
			return true;
		}

		return false;
	}
};

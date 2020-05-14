#pragma once

#include "Surface.h"

class Triangle: public Surface
{
public:
	Point3 vertex[3];

	Triangle(const Point3& p0_v, const Point3& p1_v, const Point3& p2_v): vertex{p0_v, p1_v, p2_v}
	{ }

	bool getIntersection(const Ray& r, PointRecord& hit_record) const
	{
		const float eps = 1e-6;

		const Vec3& e_1 = vertex[1] - vertex[0];
		const Vec3& e_2 = vertex[2] - vertex[0];

		const Vec3& n = e_1.cross(e_2) / e_1.cross(e_2).length();

		const Vec3& q = r.dir.cross(e_2);
		const float a = dot(e_1, q);

		if ((dot(n, r.dir) >= 0) || (abs(a) <= eps)) return false;

		const Vec3& s = (r.origin - vertex[0]) / a;
		const Vec3& ro = s.cross(e_1);

		float b[3];
		b[0] = dot(s, q);
		b[1] = dot(ro, r.dir);
		b[2] = 1.0f - b[0] - b[1];

		if ((b[0] < 0.0f) || (b[1] < 0.0f) || (b[2] < 0.0f)) return false;

		float t = dot(e_2, ro);

		if (t >= 0.0f) {
			hit_record.pos = r.pointAt(t);
			return true;
		}

		return false;
	}
};

#include "Plane.h"

Plane::Plane(Point3 pos_v, Vec3 normal_v): pos(pos_v), normal(normal_v)
{ 
	normal = normal / normal.length();
}

void Plane::shift(const Vec3& offset)
{
	pos += offset;
	return;
}

void Plane::scale(const float k, const Point3& o)
{
	pos = k*(pos - o) + o;
	return;
}

bool Plane::hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const
{
	Vec3 v = pos - r.origin;
	float cos_angle = dot(r.dir, normal);	

	if (cos_angle < 0)
	{
		float s = dot(v, normal) / cos_angle;		

		if (s >= s_min && s < s_max)
		{
			hit_record.pos = r.pointAt(s);
			hit_record.normal = normal;

			return true;
		}
	}

	return false;
}
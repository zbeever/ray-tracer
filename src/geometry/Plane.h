#pragma once

#include "Surface.h"

class Plane: public Surface
{
private:
	Point3 pos;
	Vec3 normal;

public:
	Plane(Point3 pos_v, Vec3 normal_v): pos(pos_v), normal(normal_v)
	{ 
		normal = normal / normal.length();
	}

	bool getIntersection(Ray r, PointRecord& hit_record) const
	{
		Vec3 v = pos - r.origin;
		float cos_angle = dot(r.dir, normal);	
	
		if (cos_angle < 0) {
			float s = dot(v, normal) / cos_angle;		

			hit_record.pos = r.pointAt(s);
			hit_record.normal = normal;

			return true;
		}

		return false;
	}
};

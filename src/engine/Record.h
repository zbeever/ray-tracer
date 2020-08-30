#pragma once

#include <memory>
#include "Ray.h"
#include "PDF.h"

class Material;

struct Record
{
	Point3 p;
	Vec3 normal;
	std::shared_ptr<Material> mat_ptr;
	double t;
	double u;
	double v;
	bool front_face;

	inline void set_face_normal(const Ray& r, const Vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0.0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

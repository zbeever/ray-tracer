#pragma once

#include <memory>
#include "../geometry/Ray.h"
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
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

struct ScatterRecord
{
	Ray specular_ray;
	bool is_specular;
	Color attenuation;
	std::shared_ptr<PDF> pdf_ptr;
};

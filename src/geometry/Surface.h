#pragma once

#include <memory>
#include "Ray.h"
#include "../materials/Material.h"

struct HitRecord
{
	float s;

	Point3 pos;
	Vec3 normal;

	std::shared_ptr<Material> mat;
};

class Surface
{
public:
	virtual bool hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const = 0;

	virtual void shift(const Vec3& offset) = 0;
	virtual void scale(const float k, const Point3& o) = 0;
};

#pragma once

#include "PointRecord.h"
#include "Ray.h"

class Surface
{
public:
	virtual bool getIntersection(const Ray& r, PointRecord& hit_record) const = 0;

	virtual void shift(const Vec3& offset) = 0;
	virtual void scale(const float k) = 0;
	// virtual void rotate(const Point3& p, const Vec3& axis, const float deg) = 0;
};

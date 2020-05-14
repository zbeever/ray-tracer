#pragma once

#include "PointRecord.h"
#include "Ray.h"

class Surface
{
public:
	bool getIntersection(const Ray& r, PointRecord& hit_record) const;
};

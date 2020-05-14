#pragma once

#include "Ray.h"

class PointRecord
{
public:
	Point3 pos;
	Vec3 normal;

	PointRecord(Point3 pos_v, Vec3 normal_v): pos(pos_v), normal(normal_v)
	{ }
};

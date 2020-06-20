#pragma once

#include "../engine/Scene.h"
#include "Surface.h"
#include "AARect.h"

class Box: public Surface
{
public:
	Point3 box_min, box_max;
	Scene sides;

	Box();
	Box(const Point3& p0, const Point3& p1, std::shared_ptr<Material> ptr);

	virtual bool hit(const Ray& r, double t0, double t1, Record& rec) const;
	virtual bool bounding_box(double t0, double t1, AABB& output_box) const;
};

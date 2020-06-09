#pragma once

#include "../geometry/Surface.h"
#include "Scene.h"

class BVHNode: public Surface
{
public:
	std::shared_ptr<Surface> left;
	std::shared_ptr<Surface> right;
	AABB box;

	BVHNode();
	BVHNode(Scene& list, double time0, double time1, std::mt19937& rgen);
	BVHNode(std::vector<std::shared_ptr<Surface>>& objects, size_t start, size_t end, double time0, double time1, std::mt19937& rgen);

	virtual bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const;	
	virtual bool bounding_box(const double t0, const double t1, AABB& output_box) const;
};

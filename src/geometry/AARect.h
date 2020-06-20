#pragma once

#include "Surface.h"

class AARect: public Surface
{
public:
	std::shared_ptr<Material> mp;
	double b0, b1, c0, c1, k;
	int a_ind, b_ind, c_ind;

	AARect();
	AARect(char _axis, double _b0, double _b1, double _c0, double _c1, double _k, std::shared_ptr<Material> mat);

	virtual bool hit(const Ray& r, double t0, double t1, Record& rec) const;
	virtual bool bounding_box(double t0, double t1, AABB& output_box) const;
};

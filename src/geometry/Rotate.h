#pragma once

#include "Surface.h"

class Rotate: public Surface
{
public:
	std::shared_ptr<Surface> ptr;
	double sin_theta;
	double cos_theta;
	bool has_box;
	AABB bbox;
	int a_ind, b_ind, c_ind;

	Rotate(std::shared_ptr<Surface> p, char _axis, double angle);

	virtual bool hit(const Ray& r, double t_min, double t_max, Record& rec) const;
	virtual bool bounding_box(double t0, double t1, AABB& output_box) const;
};

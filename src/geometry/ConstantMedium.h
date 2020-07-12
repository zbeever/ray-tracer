#pragma once

#include "Surface.h"
#include "../materials/Isotropic.h"

class ConstantMedium: public Surface
{
public:
	std::shared_ptr<Surface> boundary;
	std::shared_ptr<Material> phase_function;
	double neg_inv_density;

	ConstantMedium(std::shared_ptr<Surface> b, double d, std::shared_ptr<Texture> a);

	virtual bool hit(const Ray& r, double t_min, double t_max, Record& rec, std::mt19937& rgen) const;
	virtual bool bounding_box(double t0, double t1, AABB& output_box) const;
};

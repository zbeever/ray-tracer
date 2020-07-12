#pragma once

#include <random>

#include "../engine/Record.h"
#include "AABB.h"

class Surface
{
private:
	bool flip_normals = false;

public:
	virtual bool hit(const Ray& r, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const = 0;
	virtual bool bounding_box(const double t0, const double t1, AABB& output_box) const = 0;

	virtual double pdf_value(const Point3& o, const Vec3& v, std::mt19937& rgen) const
	{
		return 0.0;
	}

	virtual Vec3 random(const Vec3& o, std::mt19937& rgen) const
	{
		return Vec3(1, 0, 0);
	}
};

bool box_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b, int axis);
bool box_x_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b);
bool box_y_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b);
bool box_z_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b);

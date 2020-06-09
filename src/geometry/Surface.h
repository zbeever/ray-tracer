#pragma once

#include "../engine/Record.h"
#include "AABB.h"

class Surface
{
public:
	virtual bool hit(const Ray& r, const double t_min, const double t_max, Record& rec) const = 0;
	virtual bool bounding_box(const double t0, const double t1, AABB& output_box) const = 0;
};

bool box_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b, int axis);
bool box_x_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b);
bool box_y_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b);
bool box_z_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b);

#include "Surface.h"

bool box_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b, int axis)
{
	AABB box_a, box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
	{
		std::cerr << "No bounding box in BVHNode constructor.\n";
	}

	return box_a.min()[axis] < box_b.min()[axis];
}


bool box_x_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b)
{
	return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b)
{
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<Surface> a, const std::shared_ptr<Surface> b)
{
	return box_compare(a, b, 2);
}

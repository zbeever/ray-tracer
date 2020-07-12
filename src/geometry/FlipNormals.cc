#include "FlipNormals.h"

FlipNormals::FlipNormals(std::shared_ptr<Surface> p): ptr(p)
{ }

bool FlipNormals::hit(const Ray& r, double t_min, double t_max, Record& rec, std::mt19937& rgen) const
{
	if (!ptr->hit(r, t_min, t_max, rec, rgen))
	{
		return false;
	}

	rec.front_face = !rec.front_face;
	return true;
}

bool FlipNormals::bounding_box(double t0, double t1, AABB& output_box) const
{
	return ptr->bounding_box(t0, t1, output_box);
}

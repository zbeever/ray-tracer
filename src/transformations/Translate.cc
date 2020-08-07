#include "Translate.h"

Translate::Translate(std::shared_ptr<Surface> p, const Vec3& displacement): ptr(p), offset(displacement)
{ }

bool Translate::hit(const Ray& r, double t_min, double t_max, Record& rec, std::mt19937& rgen) const
{
	Ray moved_r(r.origin() - offset, r.direction(), r.time());
	if (!ptr->hit(moved_r, t_min, t_max, rec, rgen)) {
		return false;
	}

	rec.p += offset;
	rec.set_face_normal(moved_r, rec.normal);

	return true;
}

bool Translate::bounding_box(double t0, double t1, AABB& output_box) const
{
	if (!ptr->bounding_box(t0, t1, output_box)) {
		return false;
	}

	output_box = AABB(output_box.min() + offset, output_box.max() + offset);
	return true;
}

std::shared_ptr<Translate> Translate::applyTo(std::shared_ptr<Surface> p, const Vec3& displacement)
{
	return std::make_shared<Translate>(p, displacement);
}

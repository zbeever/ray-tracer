#pragma once

#include "../geometry/Surface.h"
#include "../materials/Material.h"

class Object
{
public:
	std::shared_ptr<Surface> surf;
	std::shared_ptr<Material> mat;

	Object(const std::shared_ptr<Surface>& surf_v, const std::shared_ptr<Material>& mat_v): surf(surf_v), mat(mat_v)
	{ }

	bool hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const
	{
		if (surf->hit(r, hit_record, s_min, s_max))
		{
			hit_record.mat = mat;
			return true;
		}
		return false;
	}
};

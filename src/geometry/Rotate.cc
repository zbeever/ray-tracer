#include "Rotate.h"

Rotate::Rotate(std::shared_ptr<Surface> p, char _axis, double angle)
{
	if (_axis == 'x')
	{
		a_ind = 0;
		b_ind = 1;
		c_ind = 2;
	}
	else if (_axis == 'y')
	{
		a_ind = 1;
		b_ind = 0;
		c_ind = 2;
	}
	else if (_axis == 'z')
	{
		a_ind = 2;
		b_ind = 0;
		c_ind = 1;
	}

	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	has_box = ptr->bounding_box(0, 1, bbox);

	Point3 min(infinity, infinity, infinity);
	Point3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Point3 old_point(i * bbox.max().x() + (1 - i) * bbox.min().x(),
						 j * bbox.max().y() + (1 - j) * bbox.min().y(),
						 k * bbox.max().z() + (1 - k) * bbox.min().z());

				Point3 new_point(0, 0, 0);
				new_point[b_ind] = cos_theta * old_point[b_ind] + sin_theta * old_point[c_ind];
				new_point[c_ind] = -sin_theta * old_point[b_ind] + cos_theta * old_point[c_ind];

				Point3 tester(0, 0, 0);
				tester[a_ind] = old_point[a_ind];
				tester[b_ind] = new_point[b_ind];
				tester[c_ind] = new_point[c_ind];

				for (int c = 0; c < 3; ++c)
				{
					min[c] = std::min(min[c], tester[c]);
					max[c] = std::max(max[c], tester[c]);
				}
			}
		}
	}


	bbox = AABB(min, max);
}

bool Rotate::hit(const Ray& r, double t_min, double t_max, Record& rec) const
{
	auto origin = r.origin();
	auto direction = r.direction();

	origin[b_ind] = cos_theta * r.origin()[b_ind] - sin_theta * r.origin()[c_ind];
	origin[c_ind] = sin_theta * r.origin()[b_ind] + cos_theta * r.origin()[c_ind];

	direction[b_ind] = cos_theta * r.direction()[b_ind] - sin_theta * r.direction()[c_ind];
	direction[c_ind] = sin_theta * r.direction()[b_ind] + cos_theta * r.direction()[c_ind];

	Ray rotated_r(origin, direction, r.time());

	if (!ptr->hit(rotated_r, t_min, t_max, rec))
	{
		return false;
	}

	auto p = rec.p;
	auto normal = rec.normal;

	p[b_ind] =  cos_theta * rec.p[b_ind] + sin_theta * rec.p[c_ind];
	p[c_ind] = -sin_theta * rec.p[b_ind] + cos_theta * rec.p[c_ind];

	normal[b_ind] =  cos_theta * rec.normal[b_ind] + sin_theta * rec.normal[c_ind];
	normal[c_ind] = -sin_theta * rec.normal[b_ind] + cos_theta * rec.normal[c_ind];

	rec.p = p;
	rec.set_face_normal(rotated_r, normal);

	return true;
}

bool Rotate::bounding_box(double t0, double t1, AABB& output_box) const
{
	output_box = bbox;
	return has_box;
}

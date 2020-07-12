#include "AARect.h"

AARect::AARect()
{ }

AARect::AARect(char _axis, double _b0, double _b1, double _c0, double _c1, double _k, std::shared_ptr<Material> mat): b0(_b0), b1(_b1), c0(_c0), c1(_c1), k(_k), mp(mat)
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
}

bool AARect::hit(const Ray& r, double t0, double t1, Record& rec, std::mt19937& rgen) const
{
	// Check if the rectangle is in front of the ray
	auto t = (k - r.origin()[a_ind]) / r.direction()[a_ind];
	if (t < t0 || t > t1)
	{
		return false;
	}

	// Check if the intersection point falls within the rectangle
	double b = r.origin()[b_ind] + t * r.direction()[b_ind];
	double c = r.origin()[c_ind] + t * r.direction()[c_ind];
	// std::cerr << b << " " << c << "\n" << std::flush;
	if (b < b0 || b > b1 || c < c0 || c > c1)
	{
		return false;
	}

	rec.u = (b - b0) / (b1 - b0);
	rec.v = (c - c0) / (c1 - c0);
	rec.t = t;

	auto outward_normal = Vec3(0., 0., 0.);
	outward_normal[a_ind] = 1.;

	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);

	return true;
}

bool AARect::bounding_box(double t0, double t1, AABB& output_box) const
{
	Point3 min_point(0., 0., 0.);
	Point3 max_point(0., 0., 0.);

	min_point[a_ind] = k - 1e-4;
	min_point[b_ind] = b0;
	min_point[c_ind] = c0;

	max_point[a_ind] = k + 1e-4;
	max_point[b_ind] = b1;
	max_point[c_ind] = c1;

	output_box = AABB(min_point, max_point);
	return true;
}

double AARect::pdf_value(const Point3& origin, const Vec3& v, std::mt19937& rgen) const
{
	Record rec;
	if (!this->hit(Ray(origin, v), 1e-3, infinity, rec, rgen))
		return 0;

	auto area = (b1 - b0) * (c1 - c0);
	auto distance_squared = rec.t * rec.t * v.length_squared();
	auto cosine = std::abs(dot(v, rec.normal) / v.length());

	return distance_squared / (cosine * area);
}

Vec3 AARect::random(const Vec3& origin, std::mt19937& rgen) const
{
	auto random_point = Point3(random_double(b0, b1, rgen), k, random_double(c0, c1, rgen));
	return random_point - origin;
}

#include "Vec3.h"

#include <random>

Vec3::Vec3(): e{0, 0, 0}
{ }

Vec3::Vec3(const double e0, const double e1, const double e2): e{e0, e1, e2}
{ }

double Vec3::x() const { return e[0]; }
double Vec3::y() const { return e[1]; }
double Vec3::z() const { return e[2]; }

double Vec3::r() const { return e[0]; }
double Vec3::g() const { return e[1]; }
double Vec3::b() const { return e[2]; }

Vec3 Vec3::operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
double Vec3::operator[](const int i) const { return e[i]; }
double& Vec3::operator[](const int i) { return e[i]; }

Vec3& Vec3::operator+=(const Vec3& v)
{
	e[0] += v[0];
	e[1] += v[1];
	e[2] += v[2];
	return *this;
}

Vec3& Vec3::operator*=(const double t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

Vec3& Vec3::operator/=(const double t)
{
	*this *= 1. / t;
	return *this;
}

double Vec3::length() const
{
	return sqrt(length_squared());
}

double Vec3::length_squared() const
{
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

Vec3 random_in_unit_sphere(std::mt19937& rgen)
{
	double u = random_double(rgen);
	double v = random_double(rgen);

	double phi = 2 * pi * v;
	double z = 1 - 2 * u;
	double r = sqrt(1 - z * z);

	return Vec3(r * cos(phi), r * sin(phi), z);
}

Vec3 random_cos_weighted_hemisphere(const Vec3& normal, std::mt19937& rgen)
{
	auto u = random_double(rgen);
	auto scaled_v = random_double(0, 2 * pi, rgen);
	
	Vec3 initial(sqrt(u) * cos(scaled_v), sqrt(u) * sin(scaled_v), sqrt(1 - u));

	auto phi = atan2(normal.y(), normal.x());
	auto theta = acos(normal.z() / normal.length());

	Vec3 rotate_about_y(cos(theta) * initial.x() + sin(theta) * initial.z(), initial.y(), -sin(theta) * initial.x() + cos(theta) * initial.z());
	Vec3 rotate_about_z(cos(phi) * rotate_about_y.x() - sin(phi) * rotate_about_y.y(), sin(phi) * rotate_about_y.x() + cos(phi) * rotate_about_y.y(), rotate_about_y.z());

	return rotate_about_z;
}

Vec3 random_unit_vector(std::mt19937& rgen)
{
	auto a = random_double(0, 2 * pi, rgen);
	auto z = random_double(-1, 1, rgen);
	auto r = sqrt(1 - z * z);

	return Vec3(r * cos(a), r * sin(a), z);
}

Vec3 random_in_hemisphere(const Vec3& normal, std::mt19937& rgen)
{
	Vec3 in_unit_sphere = random_in_unit_sphere(rgen);
	if (dot(in_unit_sphere, normal) > 0.0)
	{
		return in_unit_sphere;
	}
	else
	{
		return -in_unit_sphere;
	}
}

Vec3 random_in_unit_disc(std::mt19937& rgen)
{
	double u = random_double(rgen);
	double v = random_double(rgen);
	
	double r = sqrt(u);
	double phi = 2 * pi * v;

	return Vec3(r * cos(phi), r * sin(phi), 0);
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

Vec3 refract(const Vec3& uv, const Vec3& n, const double etai_over_etat)
{
	auto cos_theta = dot(-uv, n);
	Vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
	Vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;
	return r_out_parallel + r_out_perp;
}

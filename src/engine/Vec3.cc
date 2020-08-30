#include "Vec3.h"

Vec3::Vec3(): e{0, 0, 0}
{ }

Vec3::Vec3(const double e0, const double e1, const double e2): e{e0, e1, e2}
{ }

double Vec3::x() const
{
	return e[0];
}

double Vec3::y() const
{
	return e[1];
}

double Vec3::z() const
{
	return e[2];
}

double Vec3::r() const
{
	return e[0];
}

double Vec3::g() const
{
	return e[1];
}

double Vec3::b() const
{
	return e[2];
}

Vec3 Vec3::operator-() const
{
	return Vec3(-e[0], -e[1], -e[2]);
}

double Vec3::operator[](int i) const
{
	return e[i];
}

double& Vec3::operator[](int i)
{
	return e[i];
}

Vec3& Vec3::operator+=(const Vec3& v)
{
	e[0] += v[0];
	e[1] += v[1];
	e[2] += v[2];
	return *this;
}

Vec3& Vec3::operator*=(double t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

Vec3& Vec3::operator/=(double t)
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

	double phi = 2 * PI * v;
	double z = 1 - 2 * u;
	double r = sqrt(1 - z * z);

	return Vec3(r * cos(phi), r * sin(phi), z);
}

Vec3 random_cos_weighted_hemisphere(std::mt19937& rgen)
{
	auto u = random_double(rgen);
	auto sqrt_u = sqrt(u);
	auto scaled_v = random_double(0, 2 * PI, rgen);
	
	return Vec3(sqrt_u * cos(scaled_v), sqrt_u * sin(scaled_v), sqrt(1 - u));
}

Vec3 random_unit_vector(std::mt19937& rgen)
{
	auto a = random_double(0, 2 * PI, rgen);
	auto z = random_double(-1, 1, rgen);
	auto r = sqrt(1 - z * z);

	return Vec3(r * cos(a), r * sin(a), z);
}

Vec3 random_in_unit_disc(std::mt19937& rgen)
{
	double u = random_double(rgen);
	double v = random_double(rgen);
	
	double r = sqrt(u);
	double phi = 2 * PI * v;

	return Vec3(r * cos(phi), r * sin(phi), 0);
}

Vec3 random_in_beckmann(std::mt19937& rgen, const double m)
{
	double u = random_double(rgen);
	double v = random_double(rgen);

	double theta = M_PI / 2.;
	if (u < 1.) {
		theta = atan(sqrt(-pow(m, 2.0) * log(1.0 - u)));
	}
	double phi = 2 * M_PI * v;

	double sinTheta = sin(theta);
	double cosTheta = cos(theta);
	double sinPhi = sin(phi);
	double cosPhi = cos(phi);

	return Vec3(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
}


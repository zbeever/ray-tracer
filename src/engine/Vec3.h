#pragma once

#include <cmath>
#include <iostream>
#include <array>
#include <random>
#include "Utils.h"

class Vec3
{
public:
	std::array<double, 3> e;

	Vec3();

	Vec3(double e0, double e1, double e2);

	double x() const;
	double y() const;
	double z() const;

	double r() const;
	double g() const;
	double b() const;

	Vec3 operator-() const;
	double operator[](int i) const;
	double& operator[](int i);

	Vec3& operator+=(const Vec3& v);
	Vec3& operator*=(double t);
	Vec3& operator/=(double t);

	double length() const;
	double length_squared() const;
};


inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
	return (1. / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v)
{
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u[1] * v[2] - u[2] * v[1],
		    u[2] * v[0] - u[0] * v[2],
		    u[0] * v[1] - u[1] * v[0]);
}

inline Vec3 normalize(const Vec3& v)
{
	return v / v.length();
}

inline bool is_zero(const Vec3& v)
{
	double eps = 1e-6;
	if (std::abs(v.x()) < eps && std::abs(v.y()) < eps && std::abs(v.z()) < eps) {
		return true;
	}
	return false;
}

Vec3 random_in_unit_sphere(std::mt19937& rgen);
Vec3 random_unit_vector(std::mt19937& rgen);
Vec3 random_cos_weighted_hemisphere(std::mt19937& rgen);
Vec3 random_in_unit_disc(std::mt19937& rgen);
Vec3 random_in_beckmann(std::mt19937& rgen, const double m);

typedef Vec3 Point3;
typedef Vec3 Color;

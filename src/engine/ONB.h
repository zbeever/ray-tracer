#pragma once

#include "Vec3.h"

class ONB
{
public:
	Vec3 axis[3];

	ONB();

	inline Vec3 operator[](const int i) const
	{
		return axis[i];
	}

	Vec3 u() const;
	Vec3 v() const;
	Vec3 w() const;

	Vec3 global_to_local(const Vec3& a) const
	{
		return Vec3(dot(a, u()), dot(a, v()), dot(a, w()));
	}

	Vec3 local(const double a, const double b, const double c) const;
	Vec3 local(const Vec3& a) const;

	void build_from_w(const Vec3& w);
};

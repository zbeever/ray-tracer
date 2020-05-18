#pragma once

#include <stdlib.h>
#include <cmath>
#include <vector>

class Vec3
{
public:
	float x, y, z;

	Vec3(float x_v, float y_v, float z_v);
	Vec3(const Vec3& v);
	Vec3(const std::vector<float> v);
	Vec3();

	float length() const;
	Vec3 direction() const;

	Vec3 operator+(const Vec3& v) const;
	Vec3 operator+=(const Vec3& v);
	Vec3 operator*=(const float k);
	Vec3 operator-(const Vec3& v) const;
	Vec3 operator-() const;
	Vec3 operator*(float k) const;
	Vec3 operator/(float k) const;
	float operator[](int i) const;
	Vec3 cross(const Vec3& w) const;
};

Vec3 operator*(const float k, const Vec3& v);

float dot(const Vec3& v, const Vec3& w);

typedef Vec3 Point3;

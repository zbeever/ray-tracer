#include "Vec3.h"

Vec3::Vec3(float x_v, float y_v, float z_v): x(x_v), y(y_v), z(z_v)
{ }

Vec3::Vec3(const Vec3& v): x(v.x), y(v.y), z(v.z)
{ }

Vec3::Vec3(const std::vector<float> v): x(v[0]), y(v[1]), z(v[2])
{ }

Vec3::Vec3(): x(0), y(0), z(0)
{ }

float Vec3::length() const
{
	return std::sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::direction() const
{
	float k = 1.0f / this->length(); 
	return Vec3(k*x, y*x, z*x);
}

Vec3 Vec3::operator+(const Vec3& v) const
{
	return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator+=(const Vec3& v)
{
	this->x += v.x; this->y += v.y; this->z += v.z;
	return *this;
}

Vec3 Vec3::operator*=(const int k)
{
	this->x *= k; this->y *= k; this->z *= k;
	return *this;
}

Vec3 Vec3::operator-(const Vec3& v) const
{
	return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator*(float k) const
{
	return Vec3(k*x, k*y, k*z);
}

Vec3 Vec3::operator/(float k) const
{
	float m = 1.0f / k;
	return Vec3(m*x, m*y, m*z);
}

float Vec3::operator[](int i) const
{
	switch(i) {
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			exit(EXIT_FAILURE);
	}
}

Vec3 Vec3::cross(const Vec3& w) const
{
	return Vec3(y*w.z - z*w.y, z*w.x - x*w.z, x*w.y - y*w.x);
}

Vec3 operator*(const float k, const Vec3& v)
{
	return Vec3(k*v.x, k*v.y, k*v.z);	
}

float dot(const Vec3& v, const Vec3& w)
{
	return v.x*w.x + v.y*w.y + v.z*w.z;
}

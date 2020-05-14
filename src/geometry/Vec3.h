#pragma once

class Vec3
{
public:
	float x, y, z;

	Vec3(float x_v, float y_v, float z_v): x(x_v), y(y_v), z(z_v)
	{ }

	Vec3(const Vec3& v): x(v.x), y(v.y), z(v.z)
	{ }

	float length() const { return std::sqrt(x*x + y*y + z*z); }

	Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator-() const { return Vec3(-x, -y, -z); }
	Vec3 operator*(float k) const { return Vec3(k*x, k*y, k*z); }
	Vec3 operator/(float k) const
	{
		float m = 1.0f / k;
		return Vec3(m*x, m*y, m*z);
	}
	float operator[](int i) const
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

};

float dot(const Vec3& v, const Vec3& w)
{
	return v.x*w.x + v.y*w.y + v.z*w.z;
}

typedef Vec3 Point3;
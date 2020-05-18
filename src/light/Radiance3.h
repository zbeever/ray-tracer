#pragma once

class Radiance3
{
public:
	float r, g, b;

	Radiance3(const float r_v, const float g_v, const float b_v): r(r_v), g(g_v), b(b_v)
	{ }

	Radiance3(const Radiance3& l): r(l.r), g(l.g), b(l.b)
	{ }

	Radiance3 operator+(const Radiance3& l) const
	{
		return Radiance3(r + l.r, g + l.g, b + l.b);
	}

	Radiance3 operator+=(const Radiance3& l)
	{
		this->r += l.r; this->g += l.g; this->b += l.b;
		return *this;
	}

	Radiance3 operator*(const float k) const
	{
		return Radiance3(k*r, k*g, k*b);
	}

	Radiance3 operator*=(const float k)
	{
		this->r *= k; this->g *= k; this->b *= k;
		return *this;
	}

	Radiance3 operator-(const Radiance3& l) const
	{
		return Radiance3(r - l.r, g - l.g, b - l.b);
	}

	Radiance3 operator/(const float k) const
	{
		float m = 1.0f / k;
		return Radiance3(m*r, m*g, m*b);
	}
};


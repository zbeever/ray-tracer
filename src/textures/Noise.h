#pragma once

#include "Perlin.h"

class Noise: public Texture
{
public:
	Perlin noise;
	double scale;

	Noise(double scale_): scale(scale_)
	{ }

	virtual Color value(double u, double v, const Point3& p) const
	{
		return Color(1.0, 1.0, 1.0) * 0.5 * (1.0 + sin(scale * p.z() + 10 * noise.turb(p)));
	}
};

#pragma once

#include "../engine/Texture.h"
#include "Perlin.h"

class Noise: public Texture
{
public:
	Perlin noise;
	double scale;

	Noise(double scale_);

	virtual Color value(double u, double v, const Point3& p) const;
	static std::shared_ptr<Noise> make(double scale_);
};

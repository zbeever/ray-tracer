#pragma once

#include <memory>
#include "Vec3.h"

#include "Spectrum.h"

class Texture
{
public:
	virtual double value(int bin, double u, double v, const Point3& p) const = 0;
};

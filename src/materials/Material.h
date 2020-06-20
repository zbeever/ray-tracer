#pragma once

#include "../geometry/Vec3.h"
#include "../engine/Record.h"
#include "../textures/Texture.h"
#include "../geometry/Ray.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const = 0;
	virtual Color emitted(double u, double v, const Point3& p) const
	{
		return Color(0.0, 0.0, 0.0);
	}
};

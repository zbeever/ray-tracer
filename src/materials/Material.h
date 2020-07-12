#pragma once

#include "../geometry/Vec3.h"
#include "../engine/Record.h"
#include "../textures/Texture.h"
#include "../geometry/Ray.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, double& pdf, std::mt19937& rgen) const
	{
		return false;
	}

	virtual double scattering_pdf(const Ray& r_in, const Record& rec, const Ray& scattered) const
	{
		return 0;
	}

	virtual Color emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const
	{
		return Color(0.0, 0.0, 0.0);
	}
};

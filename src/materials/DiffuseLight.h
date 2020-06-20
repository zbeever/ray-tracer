#pragma once

#include "Material.h"

class DiffuseLight: public Material
{
public:
	std::shared_ptr<Texture> emit;

	DiffuseLight(std::shared_ptr<Texture> a): emit(a)
	{ }

	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
	{
		return false;
	}

	virtual Color emitted(double u, double v, const Point3& p) const
	{
		return emit->value(u, v, p);
	}
};

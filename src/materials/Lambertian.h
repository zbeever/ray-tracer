#pragma once

#include "Material.h"
#include "../engine/Utils.h"

class Lambertian: public Material
{
public:
	Color albedo;

	Lambertian(const Color& albedo_);
	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const;
};

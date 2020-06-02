#pragma once

#include "Material.h"
#include "../engine/Utils.h"

class Dielectric: public Material
{
public:
	Color albedo;
	double ri;

	Dielectric(const Color& albedo_, const double ri_);
	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const;
};

double schlick(const double cosine, const double ri);

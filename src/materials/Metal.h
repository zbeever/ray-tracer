#pragma once

#include "Material.h"
#include "../engine/Utils.h"

class Metal: public Material
{
public:
	Color albedo;
	double fuzz;

	Metal(const Color& albedo_, const double fuzz_);
	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const;
};

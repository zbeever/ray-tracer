#pragma once

#include "Material.h"
#include "../engine/Utils.h"

class Dielectric: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	double ri;

	Dielectric(std::shared_ptr<Texture> albedo_, const double ri_);
	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
};

double schlick(const double cosine, const double ri);

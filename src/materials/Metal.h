#pragma once

#include "Material.h"
#include "../engine/Utils.h"

class Metal: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	double fuzz;

	Metal(std::shared_ptr<Texture> albedo_, const double fuzz_);
	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
};

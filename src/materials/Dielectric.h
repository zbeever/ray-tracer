#pragma once

#include "../engine/Material.h"
#include "../engine/Utils.h"

class Dielectric: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Spectrum> ri;

	Dielectric(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_);
	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
	virtual double bxdf(const Ray& r_in, Ray& r_out, const Record& rec, const ScatterRecord& srec) const
	{
		double weight = 1.0 / std::abs(dot(normalize(r_out.direction()), normalize(rec.normal)));
		return (dot(r_in.direction(), r_out.direction()) < 0.0) ? weight : albedo->value(r_in.bin(), rec.u, rec.v, rec.p) * weight;
	}
	static std::shared_ptr<Dielectric> make(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_);
};

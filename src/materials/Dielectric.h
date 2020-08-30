#pragma once

#include "../engine/Material.h"
#include "../engine/Utils.h"
#include "../engine/Fresnel.h"

class Dielectric: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Spectrum> ri;

	Dielectric(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_);
	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
	virtual double bxdf(const Ray& r_in, Ray& r_out, const Record& rec, const ScatterRecord& srec) const
	{
		// We never importance sample specular materials, so there is no need to include a check for
		// unreflected rays

		Vec3 unit_direction = normalize(r_in.direction());
		double cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
		double actual_ri = ri->get(r_in.bin());
		double etai_over_etat = (rec.front_face) ? (1.0 / actual_ri) : actual_ri;
		double F = schlick(cos_theta, etai_over_etat);
		
		// This would be weighted by a Fresnel term, but we are already sampling rays according to F and (1 - F)
		double weight = 1.0 / std::abs(dot(normalize(r_out.direction()), normalize(rec.normal)));
		return (dot(r_out.direction(), rec.normal) > 0.0) ? (weight) : (etai_over_etat * weight);
	}
	static std::shared_ptr<Dielectric> make(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_);
};

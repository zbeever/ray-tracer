#pragma once

#include "../engine/Material.h"
#include "../distributions/BeckmannFresnelPDF.h"

class MicrofacetTransmission: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Spectrum> ri;
	double roughness;

	MicrofacetTransmission(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_, const double roughness_): albedo(albedo_), ri(ri_), roughness(roughness_)
	{
	}

	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
	{
		srec.is_specular = false;
		double eta = ri->get(r_in.bin());
		double etai_over_etat;

		if (rec.front_face == true) {
			etai_over_etat = 1.0 / eta;	
		} else {
			etai_over_etat = eta;
		}

		srec.pdf_ptr = std::make_shared<BeckmannFresnelPDF>(rec.normal, roughness, etai_over_etat);
		return true;
	}

	virtual double bxdf(const Ray& r_in, Ray& r_out, const Record& rec, const ScatterRecord& srec) const
	{
		Vec3 wi = normalize(r_in.direction());
		Vec3 wo = normalize(r_out.direction());
		Vec3 n = normalize(rec.normal);
		Vec3 wh;

		double eta = ri->get(r_in.bin());
		double etai_over_etat;

		if (rec.front_face == true) {
			etai_over_etat = 1.0 / eta;	
		} else {
			etai_over_etat = eta;
		}

		if (dot(wi, wo) < 0.0) {
			wh = (wo - wi) / 2.0;
		} else {
			wh = normalize(wi - etai_over_etat * wo);
		}

		double m = roughness;

		double n_dot_h2 = pow(dot(n, wh), 2.0);
		double D = exp((n_dot_h2 - 1) / (pow(m, 2.0) * n_dot_h2)) / (PI * pow(m, 2.0) * pow(n_dot_h2, 2.0));

		double c_spec = albedo->value(r_in.bin(), rec.u, rec.v, rec.p);
		double F = schlick(std::abs(dot(wo, wh)), etai_over_etat);
		
		// THIS IS THE ISSUE
		double G = std::min(1.0, std::min(2.0 * std::abs(dot(n, wh) * dot(wi, n) / dot(wi, wh)), 2.0 * std::abs(dot(n, wh) * dot(n, wo) / dot(wi, wh))));

		return ( (D * F * G) / (4.0 * dot(n, wi) * dot(n, wo)) );

		double val = (D * G * (1.0 - F) * std::abs(dot(wo, wh)) * std::abs(dot(wi, wh))) / (pow(dot(wi, wh) * (1.0 / etai_over_etat) * dot(wo, wh), 2.0) * std::abs(dot(wi, n) * dot(wo, n)));


		if (G > 1.0) {
			std::cout << "WRONG" << std::endl;
		}
		return val;
	}
};

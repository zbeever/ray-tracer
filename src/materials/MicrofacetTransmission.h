#pragma once

#include "../engine/Material.h"
#include "../distributions/UniformPDF.h"
#include "../distributions/BeckmannPDF.h"

class MicrofacetTransmission: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Spectrum> ri;
	double roughness;

	TorranceSparrow(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_, const double roughness_): albedo(albedo_), ri_(ri), roughness(roughness_)
	{
	}

	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
	{
		srec.is_specular = false;
		srec.pdf_ptr = std::make_shared<BeckmannPDF>(rec.normal, roughness);
		return true;
	}

	virtual double bxdf(const Ray& r_in, Ray& r_out, const Record& rec, const ScatterRecord& srec) const
	{
		Vec3 w_i = normalize(r_in.direction());
		Vec3 w_o = normalize(r_out.direction());
		Vec3 n = normalize(rec.normal);
		if (dot(w_i, n) < 0.0) w_i = -w_i;
		if (dot(w_o, n) < 0.0) w_o = -w_o;
		Vec3 h = normalize(w_o + w_i);
		double m = roughness;

		double c_spec = albedo->value(r_in.bin(), rec.u, rec.v, rec.p);

		double n_dot_h2 = pow(dot(n, h), 2.0);
		double D_new = exp((n_dot_h2 - 1) / (pow(m, 2.0) * n_dot_h2)) / (PI * pow(m, 2.0) * pow(n_dot_h2, 2.0));

		double F = c_spec + (1.0 - c_spec) * pow((1.0 - dot(w_i, h)), 5.0);
		
		double G = std::min(1.0, std::min(2 * dot(n, h) * dot(n, w_o) / dot(w_o, h), 2 * dot(n, h) * dot(n, w_i) / dot(w_o, h)));

		return ( (D_new * F * G) / (4.0 * dot(n, w_i) * dot(n, w_o)) );
	}
};

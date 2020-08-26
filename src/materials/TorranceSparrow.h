#pragma once

#include "../engine/Material.h"
#include "../distributions/UniformPDF.h"
#include "../distributions/BeckmannPDF.h"

class TorranceSparrow: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Spectrum> n;
	std::shared_ptr<Spectrum> k;
	double roughness;

	TorranceSparrow(std::shared_ptr<Texture> albedo_, const double alpha_, std::shared_ptr<Spectrum> n_, std::shared_ptr<Spectrum> k_): albedo(albedo_), roughness(alpha_), n(n_), k(k_)
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
		Vec3 incident = normalize(r_in.direction());
		Vec3 exitant = normalize(r_out.direction());
		Vec3 normal = normalize(rec.normal);

		if (dot(incident, normal) < 0.0) {
			incident *= -1.0;
		}

		if (dot(exitant, normal) < 0.0) {
			return 0.0;
		}

		Vec3 H = normalize(exitant + incident);

		/* If the ray was not importance-sampled, we need to take into account the distribution. */
		double D = 1.0;
		/* Get the half angle vector's angle with the normal. */
		double alpha = acos(dot(H, normal));

		/* Compute the Beckmann distribution. */
		D = exp(-pow(tanf(alpha) / roughness, 2.0));

		/* Align the normal with the incident vector. */
		if (dot(incident, normal) > 0.0) normal = -normal;

		/* Compute the theoretical reflected and refracted angles. */
		double cosI = std::abs(dot(incident, normal));
		// double cosT = sqrtf(1.0 - pow(n1 / n2, 2.0) * (1.0 - pow(cosI, 2.0)));

		/* Compute the Fresnel term for the incident vector. */
		double cos_theta = std::abs(dot(incident, H));
		// double F = schlick(cos_theta, n->get(r_in.bin()));
		double F = fr_conductor(cos_theta, 1.0, n->get(r_in.bin()), k->get(r_in.bin()));

		/* Compute the geometric attenuation term. */
		double NdL = std::abs(dot(normal, exitant));
		double VdH = std::abs(dot(incident, H));
		double NdH = std::abs(dot(normal, H));
		double NdV = cosI;
		double G = std::min(1.0, std::min(2.0 * NdH * NdV / VdH, 2.0 * NdH * NdL / VdH));


		/* Compute the microfacet normalization term. */
		double norm = 1.0 / (M_PI * pow(roughness, 2.0) * pow(NdH, 4.0));

		/* Compute the reflectance (note the lambertian term cancels a dot product out). */
		return norm * albedo->value(r_in.bin(), rec.u, rec.v, rec.p) * (F * D * G) / (NdV);
	}
};


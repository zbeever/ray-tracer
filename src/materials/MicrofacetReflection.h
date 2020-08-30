#pragma once

#include "../engine/Material.h"
#include "../distributions/BeckmannPDF.h"
#include "../engine/Fresnel.h"
#include "../engine/Microfacet.h"

class MicrofacetReflection: public Material
{
public:
	std::shared_ptr<Spectrum> eta;
	std::shared_ptr<Spectrum> k;
	double roughness;

	MicrofacetReflection(std::shared_ptr<Spectrum> eta_, std::shared_ptr<Spectrum> k_, const double roughness_): eta(eta_), k(k_), roughness(roughness_)
	{
	}

	virtual bool scatter(const Ray& incident, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
	{
		srec.is_specular = false;
		srec.reflected = true;
		srec.pdf_ptr = std::make_shared<BeckmannPDF>(rec.normal, roughness);
		return true;
	}

	virtual double bxdf(const Ray& incident, Ray& exitant, const Record& rec, const ScatterRecord& srec) const
	{
		// Set vectors to conform to the geometry outlined in Walter et al. (2007)
		Vec3 i = -normalize(incident.direction());
		Vec3 o = normalize(exitant.direction());
		Vec3 n = normalize(rec.normal);

		if (dot(o, n) < 0.) {
			return 0.;
		}

		// Get the half-angle vector
		Vec3 h = normalize(i + o);

		// Set the indices of refraction
		double eta_i = 1.;
		double eta_o = 1.;
		
		if (rec.front_face) {
			// Entering the object
			eta_o = eta->get(incident.bin());
		} else {
			// Exiting the object
			eta_i = eta->get(incident.bin());
		}

		// The conducting Fresnel term
		double F = fresnel_conductor(dot(i, h), eta_i, eta_o, k->get(incident.bin()));

		// The microfacet distribution
		double D = D_beckmann(h, n, roughness);

		// The associated masking function 
		double G = G_beckmann(i, o, h, n, roughness);

		// The full BRDF
		return (F * D * G) / (std::abs(dot(o, n)) * std::abs(dot(i, n)));
	}
};


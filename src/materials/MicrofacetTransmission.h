#pragma once

#include "../engine/Material.h"
#include "../engine/Fresnel.h"
#include "../engine/Microfacet.h"

double F(const Vec3& i, const Vec3& m, double etaI, double etaT) {
	double etaT_over_etaI = etaT / etaI;
	double c = std::abs(dot(i, m));

	double g = pow(etaT_over_etaI, 2) - 1.0 + pow(c, 2);
	if (g < 0.) {
		return 1.;
	}
	g = sqrt(g);

	return (1. / 2.) * (pow(g - c, 2) / pow(g + c, 2)) * (1.0 + pow(c * (g + c) - 1.0, 2) / pow(c * (g - c) + 1.0, 2));
}

double D(const Vec3& m, const Vec3& n, double alpha_b) {
	if (dot(m, n) <= 0.) {
		return 0.;
	}

	double cos_theta_m = dot(m, n);
	double tan2_theta_m = (1. - pow(cos_theta_m, 2)) / pow(cos_theta_m, 2);

	return exp(-tan2_theta_m / pow(alpha_b, 2)) / (M_PI * pow(alpha_b, 2) * pow(cos_theta_m, 4));
}

class MicrofacetTransmission: public Material
{
public:
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Spectrum> eta;
	double roughness;

	MicrofacetTransmission(std::shared_ptr<Texture> albedo_, const double roughness_, std::shared_ptr<Spectrum> eta_): albedo(albedo_), roughness(roughness_), eta(eta_)
	{
	}

	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
	{
		Vec3 n = normalize(rec.normal);
		Vec3 i = -normalize(r_in.direction()); // Unsure about this one!

		double alpha_b = roughness;

		ONB uvw;
		uvw.build_from_w(n);

		Vec3 m = normalize(uvw.local(random_in_beckmann(rgen, alpha_b)));

		double etaI = 1.0;
		double etaT = 1.0;

		if (rec.front_face) {
			etaT = eta->get(r_in.bin());
		} else {
			etaI = eta->get(r_in.bin());
		}

		double etaI_over_etaT = etaI / etaT;
		double f = schlick(std::abs(dot(i, m)), etaI_over_etaT);

		if (random_double(rgen) < f) {
			Vec3 reflected = normalize(reflect(-i, m)); // THE PROBLEM STARTS HERE IN GOING FROM n TO m

			srec.specular_ray = Ray(rec.p, reflected, r_in.time(), r_in.bin());
			srec.reflected = true;
		} else {
			Vec3 refracted = normalize(refract(-i, m, etaI, etaT));

			srec.specular_ray = Ray(rec.p, refracted, r_in.time(), r_in.bin());
			srec.reflected = false;
		}

		srec.is_specular = true;
		srec.pdf_ptr = nullptr;
		return true;
	}

	virtual double bxdf(const Ray& r_in, Ray& r_out, const Record& rec, const ScatterRecord& srec) const
	{
		Vec3 n = normalize(rec.normal);
		Vec3 i = -normalize(r_in.direction()); // Unsure about this one!
		Vec3 o = normalize(r_out.direction());

		if (dot(i, n) == 0. || dot(i, o) == 0.) {
			return 0.0;
		}

		double alpha_b = roughness;

		double etaI = 1.0;
		double etaT = 1.0;

		if (rec.front_face) {
			etaI = eta->get(r_in.bin());
		} else {
			etaT = eta->get(r_in.bin());
		}

		// This would be weighted by a Fresnel term, but we are already sampling rays according to F and (1 - F)
		// double weight = 1.0 / std::abs(dot(normalize(r_out.direction()), normalize(rec.normal)));
		// return (dot(r_out.direction(), rec.normal) > 0.0) ? (weight) : (etai_over_etat * weight);

		if (srec.reflected) {
			Vec3 hr = i + o;

			if (std::abs(hr.x()) < 1e-3 && std::abs(hr.y()) < 1e-3 && std::abs(hr.z()) < 1e-3) {
				return 0.0;
			}

			hr = normalize(hr);
			// return (f * g * d) / norm;
			double g = G_beckmann(i, o, hr, n, alpha_b); // G(i, hr, n, alpha_b) * G(o, hr, n, alpha_b);
			return std::abs(dot(i, hr)) * g / (std::abs(dot(i, n)) * std::abs(dot(o, n)) * std::abs(dot(hr, n)));
		} else {
			Vec3 ht = -(i + (etaI / etaT) * o);

			if (std::abs(ht.x()) < 1e-6 && std::abs(ht.y()) < 1e-6 && std::abs(ht.z()) < 1e-6) {
				return 0.0;
			}

			ht = normalize(ht);

			if (dot(ht, n) < 0.) {
				ht *= -1.;
			}
			// return dot_prods * (pow(etaT, 2) * (1. - f) * g * d) / norm;
			double g = G_beckmann(i, o, ht, n, alpha_b); // G(i, hr, n, alpha_b) * G(o, hr, n, alpha_b);
			return std::abs(dot(i, ht)) * g / (std::abs(dot(i, n)) * std::abs(dot(o, n)) * std::abs(dot(ht, n)));
		}
	}

};


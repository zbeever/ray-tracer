#include "Dielectric.h"
#include "../engine/Fresnel.h"

Dielectric::Dielectric(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_): albedo(albedo_), ri(ri_)
{ }

bool Dielectric::scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
{
	double actual_ri = ri->get(r_in.bin());
	double etai_over_etat = (rec.front_face) ? (1.0 / actual_ri) : actual_ri;

	double eta_i = 1.;
	double eta_o = 1.;

	if (rec.front_face) {
		eta_o = ri->get(r_in.bin());
	} else {
		eta_i = ri->get(r_in.bin());
	}

	Vec3 unit_direction = normalize(r_in.direction());
	double cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

	if (etai_over_etat * sin_theta > 1.0) {
		Vec3 reflected = reflect(unit_direction, rec.normal);
		srec.specular_ray = Ray(rec.p, reflected, r_in.time(), r_in.bin());
		srec.is_specular = true;
		srec.pdf_ptr = nullptr;
		return true;
	}

	double reflect_prob = schlick(cos_theta, etai_over_etat);
	if (random_double(rgen) < reflect_prob) {
		Vec3 reflected = reflect(unit_direction, rec.normal);
		srec.specular_ray = Ray(rec.p, reflected, r_in.time(), r_in.bin());
		srec.is_specular = true;
		srec.reflected = true;
		srec.pdf_ptr = nullptr;
		return true;
	}

	Vec3 refracted = refract(unit_direction, rec.normal, eta_i, eta_o);
	srec.specular_ray = Ray(rec.p, refracted, r_in.time(), r_in.bin());
	srec.is_specular = true;
	srec.reflected = false;
	srec.pdf_ptr = nullptr;
	return true;
}

std::shared_ptr<Dielectric> Dielectric::make(std::shared_ptr<Texture> albedo_, std::shared_ptr<Spectrum> ri_)
{
	return std::make_shared<Dielectric>(albedo_, ri_);
}

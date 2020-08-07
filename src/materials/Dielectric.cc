#include "Dielectric.h"

Dielectric::Dielectric(std::shared_ptr<Texture> albedo_, const double ri_): albedo(albedo_), ri(ri_)
{ }

double schlick(const double cosine, const double ri)
{
	auto r0 = (1 - ri) / (1 + ri);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

bool Dielectric::scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
{
	double etai_over_etat = (rec.front_face) ? (1.0 / ri) : ri;

	Vec3 unit_direction = normalize(r_in.direction());
	double cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

	if (etai_over_etat * sin_theta > 1.0) {
		Vec3 reflected = reflect(unit_direction, rec.normal);
		srec.specular_ray = Ray(rec.p, reflected, r_in.time());
		srec.attenuation = Color(1., 1., 1.);
		srec.is_specular = true;
		srec.pdf_ptr = 0;
		return true;
	}

	double reflect_prob = schlick(cos_theta, etai_over_etat);
	if (random_double(rgen) < reflect_prob) {
		Vec3 reflected = reflect(unit_direction, rec.normal);
		srec.specular_ray = Ray(rec.p, reflected, r_in.time());
		srec.attenuation = Color(1., 1., 1.);
		srec.is_specular = true;
		srec.pdf_ptr = 0;
		return true;
	}

	Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
	srec.specular_ray = Ray(rec.p, refracted, r_in.time());
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	srec.is_specular = true;
	srec.pdf_ptr = 0;
	return true;
}

std::shared_ptr<Dielectric> Dielectric::make(std::shared_ptr<Texture> albedo_, const double ri_)
{
	return std::make_shared<Dielectric>(albedo_, ri_);
}

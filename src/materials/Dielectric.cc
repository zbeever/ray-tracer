#include "Dielectric.h"

Dielectric::Dielectric(const Color& albedo_, const double ri_): albedo(albedo_), ri(ri_)
{ }

double schlick(const double cosine, const double ri)
{
	auto r0 = (1 - ri) / (1 + ri);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

bool Dielectric::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
{
	double etai_over_etat = (rec.front_face) ? (1.0 / ri) : ri;

	Vec3 unit_direction = normalize(r_in.direction());
	double cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

	if (etai_over_etat * sin_theta > 1.0)
	{
		Vec3 reflected = reflect(unit_direction, rec.normal);
		scattered = Ray(rec.p, reflected, r_in.time());
		attenuation = Color(1., 1., 1.);
		return true;
	}
	double reflect_prob = schlick(cos_theta, etai_over_etat);
	if (random_double(rgen) < reflect_prob)
	{
		Vec3 reflected = reflect(unit_direction, rec.normal);
		scattered = Ray(rec.p, reflected, r_in.time());
		attenuation = Color(1., 1., 1.);
		return true;
	}

	Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
	scattered = Ray(rec.p, refracted, r_in.time());
	attenuation = albedo;
	return true;
}

#include "Lambertian.h"

Lambertian::Lambertian(std::shared_ptr<Texture> albedo_): albedo(albedo_)
{ }

bool Lambertian::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
{
	// Vec3 scattered_direction = random_in_hemisphere(rec.normal, rgen);
	Vec3 scattered_direction = random_cos_weighted_hemisphere(rec.normal, rgen);
	scattered = Ray(rec.p, scattered_direction, r_in.time());
	// attenuation = 2 * albedo->value(rec.u, rec.v, rec.p) * std::abs(dot(normalize(scattered_direction), normalize(rec.normal)));
	attenuation = albedo->value(rec.u, rec.v, rec.p);
	return true;
}

#include "Lambertian.h"

Lambertian::Lambertian(const Color& albedo_): albedo(albedo_)
{ }

bool Lambertian::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
{
	Vec3 scattered_direction = random_in_hemisphere(rec.normal, rgen);
	scattered = Ray(rec.p, scattered_direction, r_in.time());
	attenuation = 2 * albedo * std::abs(dot(normalize(scattered_direction), normalize(rec.normal)));
	return true;
}

#include "Metal.h"

Metal::Metal(std::shared_ptr<Texture> albedo_, const double fuzz_): albedo(albedo_), fuzz(fuzz_ < 1 ? fuzz_ : 1)
{ }

bool Metal::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
{
	Vec3 reflected = reflect(r_in.direction(), rec.normal);
	scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(rgen), r_in.time());
	attenuation = albedo->value(rec.u, rec.v, rec.p);
	return (dot(scattered.direction(), rec.normal) > 0); // This is here in case the 'fuzz' parameter knocks a ray below the sphere's surface
}

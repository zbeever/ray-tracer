#include "Isotropic.h"

Isotropic::Isotropic(std::shared_ptr<Texture> a): albedo(a)
{ }

bool Isotropic::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
{
	scattered = Ray(rec.p, random_in_unit_sphere(rgen), r_in.time());
	attenuation = albedo->value(rec.u, rec.v, rec.p);
	return true;
}

#include "Metal.h"

Metal::Metal(std::shared_ptr<Texture> albedo_, const double fuzz_): albedo(albedo_), fuzz(fuzz_ < 1 ? fuzz_ : 1)
{ }

bool Metal::scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
{
	Vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
	srec.specular_ray = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(rgen), r_in.time());
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	srec.is_specular = true;
	srec.pdf_ptr = 0;
	return true;
}

std::shared_ptr<Metal> Metal::make(std::shared_ptr<Texture> albedo_, const double fuzz_)
{
	return std::make_shared<Metal>(albedo_, fuzz_);
}

#include "Lambertian.h"
#include "../engine/CosinePDF.h"
#include "../geometry/ONB.h"

Lambertian::Lambertian(std::shared_ptr<Texture> albedo_): albedo(albedo_)
{ }

bool Lambertian::scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
{
	srec.is_specular = false;
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
	return true;
}

#include "Lambertian.h"
#include "../distributions/CosinePDF.h"
#include "../engine/ONB.h"

Lambertian::Lambertian(std::shared_ptr<Texture> albedo_): albedo(albedo_)
{ }

bool Lambertian::scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
{
	srec.is_specular = false;
	srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
	return true;
}

std::shared_ptr<Lambertian> Lambertian::make(std::shared_ptr<Texture> albedo_)
{
	return std::make_shared<Lambertian>(albedo_);
}

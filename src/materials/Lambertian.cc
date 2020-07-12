#include "Lambertian.h"
#include "../geometry/ONB.h"

Lambertian::Lambertian(std::shared_ptr<Texture> albedo_): albedo(albedo_)
{ }

bool Lambertian::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, double& pdf, std::mt19937& rgen) const
{
	ONB uvw;
	uvw.build_from_w(rec.normal);
	auto direction = uvw.local(random_cos_weighted_hemisphere(rgen));
	scattered = Ray(rec.p, normalize(direction), r_in.time());
	attenuation = albedo->value(rec.u, rec.v, rec.p);
	pdf = dot(uvw.w(), scattered.direction()) / pi;
	return true;
}

double Lambertian::scattering_pdf(const Ray& r_in, const Record& rec, const Ray& scattered) const
{
	auto cosine = dot(rec.normal, normalize(scattered.direction()));
	return cosine < 0.0 ? 0 : cosine / pi;
}

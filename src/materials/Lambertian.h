#pragma once

#include "Material.h"
#include "../engine/Utils.h"

class Lambertian: public Material
{
public:
	Color albedo;

	Lambertian(const Color& albedo_): albedo(albedo_)
	{ }

	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const;
};

bool Lambertian::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
{
	Vec3 scattered_direction = random_in_hemisphere(rec.normal, rgen);
	scattered = Ray(rec.p, scattered_direction);
	attenuation = 2 * albedo * std::abs(dot(normalize(scattered_direction), normalize(rec.normal)));
	return true;
}

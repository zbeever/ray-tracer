#pragma once

#include "Material.h"
#include "../textures/Texture.h"
#include "../engine/Utils.h"
#include <memory>

class Lambertian: public Material
{
public:
	std::shared_ptr<Texture> albedo;

	Lambertian(std::shared_ptr<Texture> albedo_);
	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, double& pdf, std::mt19937& rgen) const;
	virtual double scattering_pdf(const Ray& r_in, const Record& rec, const Ray& scattered) const;
};

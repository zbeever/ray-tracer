#pragma once

#include "../engine/Material.h"

class Isotropic: public Material
{
public:
	std::shared_ptr<Texture> albedo;

	Isotropic(std::shared_ptr<Texture> a);

	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const;
	static std::shared_ptr<Isotropic> make(std::shared_ptr<Texture> a);
};

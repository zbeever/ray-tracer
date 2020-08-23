#pragma once

#include "../engine/Material.h"

class DiffuseLight: public Material
{
public:
	std::shared_ptr<Texture> emit;

	DiffuseLight(std::shared_ptr<Texture> a);

	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const;
	virtual double emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const;
	static std::shared_ptr<DiffuseLight> make(std::shared_ptr<Texture> a);
};

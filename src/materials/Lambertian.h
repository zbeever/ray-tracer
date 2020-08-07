#pragma once

#include <memory>
#include "../engine/Material.h"
#include "../engine/Texture.h"
#include "../engine/Utils.h"

class Lambertian: public Material
{
public:
	std::shared_ptr<Texture> albedo;

	Lambertian(std::shared_ptr<Texture> albedo_);

	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
	static std::shared_ptr<Lambertian> make(std::shared_ptr<Texture> albedo_);
};
